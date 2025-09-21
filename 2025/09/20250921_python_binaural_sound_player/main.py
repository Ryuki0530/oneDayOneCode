# main.py
from __future__ import annotations
import argparse
import sys
import math
import os
from pathlib import Path
import pygame

from audio_engine import AudioEngine
from spatializer import Spatializer

def parse_args():
    p = argparse.ArgumentParser(description="Binaural-ish Voice Player with Pygame")
    p.add_argument("--file", required=True, help="入力音声ファイル（WAV/MP3/OGGなど）")
    p.add_argument("--rate", type=int, default=None, help="ミキサのサンプリングレート（未指定なら入力/既定に合わせる）")
    p.add_argument("--chunk", type=int, default=1024, help="1チャンクあたりサンプル数")
    p.add_argument("--lpf", action="store_true", help="距離依存の簡易LPFを有効化")
    return p.parse_args()

def main():
    args = parse_args()

    # Windowsで無音化する事例向けに DirectSound を優先
    os.environ.setdefault("SDL_AUDIODRIVER", "directsound")

    engine = AudioEngine(rate=args.rate, chunk=args.chunk)

    # 先に読み込み（拡張子で切替）
    src = Path(args.file)
    if src.suffix.lower() == ".wav":
        sr = engine.load_mono_wav(src)
    else:
        sr = engine.load_mono_any(src)

    # mixer 完全再初期化 → pre_init(ステレオ固定) → init
    try:
        pygame.mixer.quit()
    except Exception:
        pass
    pygame.mixer.pre_init(frequency=sr, size=-16, channels=2, buffer=4096)
    pygame.init()

    # 念押し（ズレがあればここで再init）
    init = pygame.mixer.get_init()
    if not init or init[2] != 2 or init[0] != sr or init[1] != -16:
        pygame.mixer.quit()
        pygame.mixer.init(frequency=sr, size=-16, channels=2, buffer=4096)

    # 画面
    W, H = 640, 360
    win = pygame.display.set_mode((W, H))
    pygame.display.set_caption("Binaural-ish Voice Player (ILD+ITD+Distance) - Pygame")
    font = pygame.font.Font(None, 28)
    clock = pygame.time.Clock()

    # エンジン
    sp = Spatializer(sample_rate=sr, k_atten=0.5, use_lpf=args.lpf)
    engine.set_spatializer(sp)
    engine.init_mixer()
    engine.set_volume(1.0, 1.0)  # 念のため最大

    # 初期姿勢
    azimuth = 0.0
    distance = 1.0
    engine.set_pose(azimuth, distance)

    # 先行バッファ
    engine.prime_buffers(num_queues=3)

    running = True
    info_msg = ""
    while running:
        for evt in pygame.event.get():
            if evt.type == pygame.QUIT:
                running = False
            elif evt.type == pygame.KEYDOWN:
                if evt.key in (pygame.K_ESCAPE, pygame.K_q):
                    running = False
                elif evt.key == pygame.K_r:
                    azimuth = 0.0
                    distance = 1.0
                    engine.set_pose(azimuth, distance)
                elif evt.key == pygame.K_LEFT:
                    azimuth -= 5.0
                    if azimuth < -180.0: azimuth += 360.0
                    engine.set_pose(azimuth, distance)
                elif evt.key == pygame.K_RIGHT:
                    azimuth += 5.0
                    if azimuth > 180.0: azimuth -= 360.0
                    engine.set_pose(azimuth, distance)
                elif evt.key == pygame.K_UP:
                    distance -= 0.05
                    if distance < 0.2: distance = 0.2
                    engine.set_pose(azimuth, distance)
                elif evt.key == pygame.K_DOWN:
                    distance += 0.05
                    if distance > 3.0: distance = 3.0
                    engine.set_pose(azimuth, distance)

        # オーディオ進行
        alive = engine.tick()
        info_msg = "再生中" if alive else "再生終了"

        # ITD表示用
        itd = engine.spatializer.current_itd_samples() if engine.spatializer else 0

        # HUD描画
        win.fill((12, 12, 18))

        # 方位円（簡易ビジュアル）
        cx, cy, rad = W // 2, H // 2 + 30, 90
        pygame.draw.circle(win, (60, 60, 90), (cx, cy), rad, width=2)
        # リスナーを上向き基準に描く（azimuth=0を前方）
        ang = math.radians(-azimuth + 90.0)  # 画面座標調整
        px = int(cx + rad * math.cos(ang))
        py = int(cy - rad * math.sin(ang))
        pygame.draw.line(win, (200, 200, 240), (cx, cy), (px, py), width=3)
        pygame.draw.circle(win, (200, 200, 240), (px, py), 6)

        # テキスト
        lines = [
            "Binaural-ish Player (ILD + ITD + Distance)",
            f"File      : {args.file}",
            f"SampleRate: {sr} Hz   Chunk: {engine.chunk}",
            f"Azimuth   : {azimuth:+.1f} deg   Distance: {distance:.2f} m",
            f"ITD       : {itd:+d} samples",
            f"Status    : {info_msg}",
            "Keys      : ←→ rotate, ↑↓ distance, R reset, Q/ESC quit",
        ]
        y = 20
        for li in lines:
            surf = font.render(li, True, (230, 230, 255))
            win.blit(surf, (20, y))
            y += 26

        pygame.display.flip()
        clock.tick(60)  # 60 FPS

    engine.close()
    pygame.quit()
    return 0

if __name__ == "__main__":
    sys.exit(main())
