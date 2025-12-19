# main.py
# -*- coding: utf-8 -*-
"""
CLI 10-band Graphic EQ (Offline WAV -> WAV)

- Windows想定（キー入力は msvcrt）。他OSでは簡易フォールバック（事前設定のみ）に切替。
- 入力: 16bit PCM WAV (mono/stereo), 44.1kHz or 48kHz
- 出力: 16bit PCM WAV
- 画面操作:
    ↑/↓ : バンド選択（カーソル移動）
    ←/→ : ゲイン ±1 dB（範囲: -12..+12）
    R    : 全バンド 0 dB リセット
    S    : 設定保存(eq.json)
    L    : 設定読込(eq.json)
    Enter: 現在設定でEQ適用→出力ファイルを書き出し
    Q    : 終了（処理せずに終了）

※ 係数はRBJのPeakingEQ（Biquad）。安定性と実装容易性のためSOS化してsosfiltを使用。
※ クリッピング対策: 既定はヘッドルーム(-6 dB)。--softclip指定でtanhソフトクリップに変更可。
"""

import argparse
import json
import math
import os
import sys
import wave
import struct
from typing import Tuple, List

import numpy as np
from scipy.signal import sosfilt  # 零位相を避け、実装シンプルさ重視
try:
    from scipy.signal import resample_poly  # 任意の強制リサンプリング用
except Exception:
    resample_poly = None

# --- 設定定数 ---
BANDS_HZ = [31, 62, 125, 250, 500, 1000, 2000, 4000, 8000, 16000]
GAIN_MIN_DB = -12
GAIN_MAX_DB = 12
GAIN_STEP_DB = 1
DEFAULT_Q = 1.4
DEFAULT_HEADROOM_DB = 6.0  # -6 dB
UI_BAR_WIDTH = 10          # 「■」の最大数

# Windowsの矢印キーコード（msvcrt仕様）
WIN_KEY_PREFIX = b'\xe0'
WIN_UP, WIN_DOWN, WIN_LEFT, WIN_RIGHT = 72, 80, 75, 77


def read_wav_int16(path: str) -> Tuple[np.ndarray, int, int]:
    """16-bit PCM WAVを読み込み -> (float64波形 [samples, channels], sample_rate, channels)

    例外は呼び出し側で捕捉してメッセージ化する。
    """
    with wave.open(path, 'rb') as wf:
        n_channels = wf.getnchannels()
        sampwidth = wf.getsampwidth()
        framerate = wf.getframerate()
        n_frames = wf.getnframes()
        comptype = wf.getcomptype()

        if sampwidth != 2:
            raise ValueError("Only 16-bit PCM WAV is supported.")
        if comptype != 'NONE':
            raise ValueError("Compressed WAV is not supported.")
        if framerate not in (44100, 48000):
            # 課題仕様では44.1k/48kのみ
            raise ValueError(f"Unsupported sample rate: {framerate}. Use 44100 or 48000 Hz.")

        raw = wf.readframes(n_frames)
        data = np.frombuffer(raw, dtype=np.int16)
        if n_channels == 2:
            data = data.reshape(-1, 2)
        else:
            data = data.reshape(-1, 1)

    # [-1, 1) に正規化（float64）
    audio = data.astype(np.float64) / 32768.0
    return audio, framerate, n_channels


def write_wav_int16(path: str, audio: np.ndarray, sample_rate: int):
    """float波形[-1..1]想定を16bit PCMにクリップしてWAV保存。"""
    # クリップ
    audio = np.clip(audio, -1.0, 1.0)
    # int16へ
    data_i16 = (audio * 32767.0).astype(np.int16)

    if data_i16.ndim == 1:
        n_channels = 1
        frames = data_i16
    else:
        n_channels = data_i16.shape[1]
        frames = data_i16.reshape(-1)

    with wave.open(path, 'wb') as wf:
        wf.setnchannels(n_channels)
        wf.setsampwidth(2)
        wf.setframerate(sample_rate)
        wf.writeframes(frames.tobytes())


def force_resample(audio: np.ndarray, fs_in: int, fs_out: int) -> Tuple[np.ndarray, int]:
    """必要なら任意サンプルレートに強制変換。scipyが無ければUnsupportedを返す。"""
    if fs_in == fs_out:
        return audio, fs_in
    if resample_poly is None:
        raise RuntimeError("resample_poly is unavailable. Install scipy >= 1.4.")
    # 比率を単純に推定（44.1k <-> 48k 間想定）
    if fs_in == 44100 and fs_out == 48000:
        up, down = 160, 147
    elif fs_in == 48000 and fs_out == 44100:
        up, down = 147, 160
    else:
        # 汎用近似（誤差は小さいが、理想ではない）
        from fractions import Fraction
        frac = Fraction(fs_out, fs_in).limit_denominator(1000)
        up, down = frac.numerator, frac.denominator

    if audio.ndim == 1:
        audio_rs = resample_poly(audio, up, down)
    else:
        # 各チャンネル独立
        audio_rs = np.vstack([resample_poly(audio[:, ch], up, down) for ch in range(audio.shape[1])]).T
    return audio_rs.astype(np.float64, copy=False), fs_out


def db_to_amp(db: float) -> float:
    return 10.0 ** (db / 20.0)


def headroom_gain_amp(headroom_db: float) -> float:
    return db_to_amp(-abs(headroom_db))


def peaking_eq_sos(fs: int, f0: float, gain_db: float, Q: float) -> np.ndarray:
    """RBJ Peaking EQ -> SOS（二次セクション1段）として返す。"""
    # 参考（ヒント）:
    # A = 10^(G/40), w0 = 2π f0 / fs, α = sin(w0) / (2Q)
    A = 10.0 ** (gain_db / 40.0)
    w0 = 2.0 * math.pi * (f0 / fs)
    cosw0 = math.cos(w0)
    sinw0 = math.sin(w0)
    alpha = sinw0 / (2.0 * Q)

    b0 = 1.0 + alpha * A
    b1 = -2.0 * cosw0
    b2 = 1.0 - alpha * A
    a0 = 1.0 + alpha / A
    a1 = -2.0 * cosw0
    a2 = 1.0 - alpha / A

    # 正規化
    b0 /= a0
    b1 /= a0
    b2 /= a0
    a1 /= a0
    a2 /= a0
    # SOS形式 [b0 b1 b2 a0 a1 a2] ただしa0は1
    sos = np.array([[b0, b1, b2, 1.0, a1, a2]], dtype=np.float64)
    return sos


def build_eq_sos_chain(fs: int, gains_db: List[float], q: float) -> np.ndarray:
    """全バンドのSOSを縦に連結（直列）。"""
    sos_list = []
    for f, g in zip(BANDS_HZ, gains_db):
        if abs(g) < 1e-9:
            # 0dBなら完全スキップ（数値安定性よし & 高速化）
            continue
        sos_list.append(peaking_eq_sos(fs, f, g, q))
    if not sos_list:
        # 恒等通過（バイパス）
        # sosfiltに渡す都合で、恒等IIRを1段用意
        sos = np.array([[1.0, 0.0, 0.0, 1.0, 0.0, 0.0]], dtype=np.float64)
    else:
        sos = np.vstack(sos_list)
    return sos


def apply_eq(audio: np.ndarray, fs: int, gains_db: List[float], q: float,
             softclip: bool, headroom_db: float) -> np.ndarray:
    """EQ適用 + クリッピング対策。"""
    # EQ
    sos = build_eq_sos_chain(fs, gains_db, q)
    if audio.ndim == 1:
        y = sosfilt(sos, audio.astype(np.float64, copy=False))
    else:
        y = np.zeros_like(audio, dtype=np.float64)
        for ch in range(audio.shape[1]):
            y[:, ch] = sosfilt(sos, audio[:, ch].astype(np.float64, copy=False))

    # クリップ対策
    if softclip:
        # ソフトクリップ。適度なkを選ぶ（0.95 付近で程よい）
        k = 1.0
        y = np.tanh(k * y)
    else:
        # ヘッドルーム（-6 dB既定）
        gain = headroom_gain_amp(headroom_db)
        y = y * gain

    return y


def load_json(path: str) -> List[float]:
    with open(path, 'r', encoding='utf-8') as f:
        data = json.load(f)
    gains = data.get("gains_db")
    if not isinstance(gains, list) or len(gains) != len(BANDS_HZ):
        raise ValueError("Invalid eq.json format: gains_db must be a list of length 10.")
    # 範囲チェック
    g2 = []
    for g in gains:
        gd = int(round(float(g)))
        gd = max(GAIN_MIN_DB, min(GAIN_MAX_DB, gd))
        g2.append(gd)
    return g2


def save_json(path: str, gains_db: List[float]):
    obj = {"bands_hz": BANDS_HZ, "gains_db": list(map(int, gains_db))}
    with open(path, 'w', encoding='utf-8') as f:
        json.dump(obj, f, ensure_ascii=False, indent=2)


def render_ui(gains_db: List[int], cursor_idx: int):
    """CLI上に簡易バーを描画。全角スペース混じりでもOKなよう、幅を揃える。"""
    # 周波数のラベル整形（桁揃え）
    labels = []
    for f in BANDS_HZ:
        if f >= 1000:
            labels.append(f"{f//1000}K ".rjust(4))
        else:
            labels.append(str(f).rjust(4))

    # バー描画
    lines = []
    for i, (lab, g) in enumerate(zip(labels, gains_db)):
        blocks = int(round((g - GAIN_MIN_DB) / (GAIN_MAX_DB - GAIN_MIN_DB) * UI_BAR_WIDTH))
        blocks = max(0, min(UI_BAR_WIDTH, blocks))
        bar = "■" * blocks
        pad = "　" * (UI_BAR_WIDTH - blocks)  # 全角スペースで見やすく
        cursor = "<" if i == cursor_idx else ""
        lines.append(f"{lab} |{bar}{pad}| {str(g).rjust(3)} dB {cursor}")

    ui = "\n".join(lines)
    help_line = "\n[↑/↓]バンド選択  [←/→]±1dB  [R]リセット  [S]保存  [L]読込  [Enter]処理  [Q]終了"
    print("\x1b[2J\x1b[H", end="")  # 画面クリア + カーソルホーム
    print(ui + help_line, flush=True)


def windows_ui_loop(gains_db: List[int], in_path: str, out_path: str, fs_force: int,
                    softclip: bool, q_value: float, headroom_db: float, config_path: str):
    """Windows: msvcrtで即時キー操作."""
    import msvcrt

    idx = 0
    render_ui(gains_db, idx)

    while True:
        ch = msvcrt.getch()
        if ch == WIN_KEY_PREFIX:
            # 矢印キー等
            code = ord(msvcrt.getch())
            if code == WIN_UP:
                idx = (idx - 1) % len(BANDS_HZ)
                render_ui(gains_db, idx)
            elif code == WIN_DOWN:
                idx = (idx + 1) % len(BANDS_HZ)
                render_ui(gains_db, idx)
            elif code == WIN_LEFT:
                gains_db[idx] = max(GAIN_MIN_DB, gains_db[idx] - GAIN_STEP_DB)
                render_ui(gains_db, idx)
            elif code == WIN_RIGHT:
                gains_db[idx] = min(GAIN_MAX_DB, gains_db[idx] + GAIN_STEP_DB)
                render_ui(gains_db, idx)
            continue

        # 1バイトキー
        if ch in (b'\r', b'\n'):  # Enter -> 処理実行
            print("\nApplying EQ... (this may take a few seconds)")
            try:
                audio, fs_in, chs = read_wav_int16(in_path)
                fs = fs_in
                if fs_force is not None:
                    audio, fs = force_resample(audio, fs_in, fs_force)
                y = apply_eq(audio, fs, gains_db, q_value, softclip, headroom_db)
                write_wav_int16(out_path, y, fs)
                print(f"Done. Wrote: {out_path}")
            except Exception as e:
                print(f"[ERROR] {e}")
            print("\nPress any key to continue UI...")
            msvcrt.getch()
            render_ui(gains_db, idx)
            continue

        c = ch.decode('utf-8', errors='ignore').lower()
        if c == 'q':
            print("\nQuit without processing.")
            return
        elif c == 'r':
            gains_db[:] = [0] * len(BANDS_HZ)
            render_ui(gains_db, idx)
        elif c == 's':
            try:
                save_json(config_path, gains_db)
                print(f"\nSaved: {config_path}")
            except Exception as e:
                print(f"\n[ERROR] save failed: {e}")
            print("Press any key to continue UI...")
            msvcrt.getch()
            render_ui(gains_db, idx)
        elif c == 'l':
            try:
                new_g = load_json(config_path)
                gains_db[:] = new_g
                print(f"\nLoaded: {config_path}")
            except Exception as e:
                print(f"\n[ERROR] load failed: {e}")
            print("Press any key to continue UI...")
            msvcrt.getch()
            render_ui(gains_db, idx)
        # 他キーは無視


def fallback_non_windows(gains_db: List[int], in_path: str, out_path: str, fs_force: int,
                         softclip: bool, q_value: float, headroom_db: float, config_path: str):
    """非Windows簡易フォールバック: 対話的UI無し。事前に--configでロード or 標準入力でゲイン列を受け取る。"""
    print("[INFO] Non-Windows environment detected. Fallback mode (no interactive UI).")
    print(f"Bands: {BANDS_HZ}")
    print(f"Current gains: {gains_db}")
    print("Enter 10 integers (dB) separated by space to override, or just Enter to keep:")
    line = input("> ").strip()
    if line:
        parts = line.split()
        if len(parts) == 10:
            gg = []
            for p in parts:
                gd = int(round(float(p)))
                gd = max(GAIN_MIN_DB, min(GAIN_MAX_DB, gd))
                gg.append(gd)
            gains_db[:] = gg
        else:
            print("[WARN] invalid input. Keep current gains.")

    try:
        audio, fs_in, chs = read_wav_int16(in_path)
        fs = fs_in
        if fs_force is not None:
            audio, fs = force_resample(audio, fs_in, fs_force)
        y = apply_eq(audio, fs, gains_db, q_value, softclip, headroom_db)
        write_wav_int16(out_path, y, fs)
        print(f"Done. Wrote: {out_path}")
    except Exception as e:
        print(f"[ERROR] {e}")


def parse_args():
    ap = argparse.ArgumentParser(description="CLI 10-band Graphic EQ (offline)")
    ap.add_argument("--in", dest="in_path", required=True, help="input WAV (16-bit PCM)")
    ap.add_argument("--out", dest="out_path", required=True, help="output WAV (16-bit PCM)")
    ap.add_argument("--rate", dest="force_rate", type=int, choices=[44100, 48000],
                    help="force resample to this rate (44100 or 48000)")
    ap.add_argument("--config", dest="config_path", default="eq.json", help="load/save EQ settings (JSON)")
    ap.add_argument("--softclip", action="store_true", help="use soft clipping (tanh) instead of headroom")
    ap.add_argument("--q", dest="q_value", type=float, default=DEFAULT_Q, help="fixed Q value for all bands")
    ap.add_argument("--headroom", dest="headroom_db", type=float, default=DEFAULT_HEADROOM_DB,
                    help="headroom dB when softclip is disabled (default: 6.0 -> -6 dB)")
    return ap.parse_args()


def main():
    args = parse_args()

    gains_db = [0] * len(BANDS_HZ)
    # 起動時プリセット読込（存在する場合）
    if args.config_path and os.path.exists(args.config_path):
        try:
            gains_db = load_json(args.config_path)
            print(f"[INFO] Loaded preset: {args.config_path}")
        except Exception as e:
            print(f"[WARN] Failed to load preset: {e}")

    # Windowsなら対話UI、その他はフォールバック
    is_windows = (os.name == "nt")
    try:
        if is_windows:
            windows_ui_loop(
                gains_db=gains_db,
                in_path=args.in_path,
                out_path=args.out_path,
                fs_force=args.force_rate,
                softclip=args.softclip,
                q_value=args.q_value,
                headroom_db=args.headroom_db,
                config_path=args.config_path
            )
        else:
            fallback_non_windows(
                gains_db=gains_db,
                in_path=args.in_path,
                out_path=args.out_path,
                fs_force=args.force_rate,
                softclip=args.softclip,
                q_value=args.q_value,
                headroom_db=args.headroom_db,
                config_path=args.config_path
            )
    except KeyboardInterrupt:
        print("\n[INFO] Interrupted.")

if __name__ == "__main__":
    main()


"""
※バイブコーディングにて実装
"""