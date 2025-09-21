# audio_engine.py
from __future__ import annotations
import wave
from pathlib import Path
from typing import Optional
import numpy as np
import pygame

from spatializer import Spatializer

Int16 = np.int16
Float32 = np.float32

class AudioEngine:
    def __init__(self, rate: Optional[int] = None, chunk: int = 1024, mixer_buffer: int = 4096):
        self.request_rate = rate  # Noneならファイルに合わせる
        self.chunk = int(chunk)
        self.mixer_buffer = int(mixer_buffer)

        self.mono: Optional[np.ndarray] = None  # int16
        self.sample_rate: Optional[int] = None
        self.pos = 0
        self.channel: Optional[pygame.mixer.Channel] = None
        self.spatializer: Optional[Spatializer] = None

    # ---- WAV読込（16bit, mono推奨。stereoは左右平均でモノラル化） ----
    def load_mono_wav(self, path: str | Path) -> int:
        path = Path(path)
        if not path.exists():
            raise FileNotFoundError(path)

        with wave.open(str(path), 'rb') as wf:
            n_channels = wf.getnchannels()
            sampwidth = wf.getsampwidth()
            framerate = wf.getframerate()
            nframes = wf.getnframes()

            if sampwidth != 2:
                raise ValueError("16-bit PCM WAVのみサポート（sampwidth != 2）")

            raw = wf.readframes(nframes)
            data = np.frombuffer(raw, dtype=Int16)

            if n_channels == 2:
                data = data.reshape(-1, 2).astype(np.int32)
                mono = ((data[:, 0] + data[:, 1]) // 2).astype(Int16)
            elif n_channels == 1:
                mono = data
            else:
                raise ValueError(f"未対応チャンネル数: {n_channels}")

        # サンプルレート決定：指定がなければファイルに合わせる
        self.sample_rate = int(self.request_rate or framerate)
        if self.request_rate is not None and self.request_rate != framerate:
            print(f"[warn] --rate({self.request_rate}) とWAV({framerate})が異なります。"
                  " 今回はミキサを --rate に合わせ、音程がずれる可能性があります。")

        self.mono = mono
        self.pos = 0
        return self.sample_rate

    # ---- MP3/OGGなど非WAVを pygame 経由でPCM化して取り込む ----
    def load_mono_any(self, path: str | Path) -> int:
        path = Path(path)
        if not path.exists():
            raise FileNotFoundError(path)

        # 一時mixerを使ってデコード（channels=2に変換される想定）
        try:
            pygame.mixer.quit()
        except Exception:
            pass

        target_rate = int(self.request_rate or 44100)
        pygame.mixer.init(frequency=target_rate, size=-16, channels=2, buffer=2048)

        try:
            snd = pygame.mixer.Sound(str(path))
            arr = pygame.sndarray.array(snd)
            arr = np.asarray(arr)

            # shape を (N,2) に正規化
            if arr.ndim == 1:
                arr = np.stack([arr, arr], axis=1)
            elif arr.ndim == 2:
                if arr.shape[1] == 2:
                    pass
                elif arr.shape[0] == 2:
                    arr = arr.T.copy()
                else:
                    raise ValueError(f"unexpected decoded shape: {arr.shape}")
            else:
                raise ValueError(f"unexpected decoded ndim: {arr.ndim}")

            if arr.dtype != np.int16:
                arr = arr.astype(np.int16)

            mono = ((arr[:, 0].astype(np.int32) + arr[:, 1].astype(np.int32)) // 2).astype(np.int16)
        finally:
            try:
                pygame.mixer.quit()
            except Exception:
                pass

        self.sample_rate = target_rate
        self.mono = mono
        self.pos = 0
        return self.sample_rate

    def init_mixer(self):
        """希望設定でmixerを初期化。"""
        if self.sample_rate is None:
            raise RuntimeError("load_mono_wav/load_mono_any の後に init_mixer() を呼んでください。")

        desired = (int(self.sample_rate), -16, 2)  # (freq, size, channels)
        init = pygame.mixer.get_init()
        need_reinit = True
        if init is not None:
            cur = (init[0], init[1], init[2])
            if cur == desired:
                need_reinit = False

        if need_reinit:
            try:
                pygame.mixer.quit()
            except Exception:
                pass
            pygame.mixer.init(
                frequency=desired[0],
                size=desired[1],
                channels=desired[2],
                buffer=self.mixer_buffer
            )

        if self.channel is None:
            self.channel = pygame.mixer.Channel(0)

    def set_spatializer(self, spatializer: Spatializer):
        self.spatializer = spatializer

    def set_pose(self, azimuth_deg: float, distance_m: float):
        if self.spatializer is not None:
            self.spatializer.set_pose(azimuth_deg, distance_m)

    def set_volume(self, vol_left: float, vol_right: float):
        """0.0..1.0 を想定。チャンネルが用意できた後に呼ぶこと。"""
        if self.channel is not None:
            self.channel.set_volume(max(0.0, min(1.0, vol_left)),
                                    max(0.0, min(1.0, vol_right)))

    def _next_stereo_chunk(self) -> Optional[np.ndarray]:
        if self.mono is None or self.spatializer is None:
            return None
        if self.pos >= len(self.mono):
            return None  # EOF
        end = min(self.pos + self.chunk, len(self.mono))
        mono_chunk = self.mono[self.pos:end]
        self.pos = end
        stereo = self.spatializer.process(mono_chunk)

        # 自己診断：直近チャンクのピーク
        if stereo.size > 0 and self.pos <= self.chunk * 2:
            peak = int(np.max(np.abs(stereo)))
            if peak == 0:
                print("[diag] chunk peak == 0 (データが無音の可能性)")
        return stereo

    def _make_sound_from_array(self, chunk: np.ndarray) -> pygame.mixer.Sound:
        """
        (N,2) int16 C連続の配列に正規化してからSound化。
        環境依存のエラーに備え、転置／bytesのフォールバック実装。
        """
        arr = np.asarray(chunk)
        if arr.ndim == 1:
            arr = np.stack([arr, arr], axis=1)
        if arr.ndim != 2 or arr.shape[1] != 2:
            raise ValueError(f"expected stereo array shape (N,2), got {arr.shape}")
        if arr.dtype != np.int16:
            arr = arr.astype(np.int16)
        if not arr.flags["C_CONTIGUOUS"]:
            arr = np.ascontiguousarray(arr)

        # 1) 通常経路：sndarray.make_sound((N,2))
        try:
            return pygame.sndarray.make_sound(arr)
        except ValueError:
            pass

        # 2) 転置フォールバック：((2,N))
        try:
            arr_T = np.ascontiguousarray(arr.T)
            return pygame.sndarray.make_sound(arr_T)
        except ValueError:
            pass

        # 3) 最終フォールバック：raw bytes（16bitステレオ・インターリーブ）
        return pygame.mixer.Sound(buffer=arr.tobytes())

    def prime_buffers(self, num_queues: int = 3):
        if self.channel is None:
            raise RuntimeError("init_mixer()の後に prime_buffers() を呼んでください。")
        filled = 0
        for _ in range(num_queues):
            chunk = self._next_stereo_chunk()
            if chunk is None or chunk.size == 0:
                break
            snd = self._make_sound_from_array(chunk)
            if not self.channel.get_busy() and filled == 0:
                self.channel.play(snd)
            else:
                self.channel.queue(snd)
            filled += 1
        if filled == 0:
            print("[diag] prime_buffers: 先行キュー生成ゼロ。入力長 or デコード失敗の可能性")

    def tick(self) -> bool:
        """
        再生中は True、終了（音源尽き & キュー空）で False を返す。
        """
        if self.channel is None:
            return False

        queued = self.channel.get_queue() is not None
        busy = self.channel.get_busy()

        # 再生が止まっていて、まだデータが残っているなら play で再開
        if not busy:
            chunk = self._next_stereo_chunk()
            if chunk is not None and chunk.size > 0:
                snd = self._make_sound_from_array(chunk)
                self.channel.play(snd)
                # 次を一つ先行
                chunk2 = self._next_stereo_chunk()
                if chunk2 is not None and chunk2.size > 0:
                    snd2 = self._make_sound_from_array(chunk2)
                    self.channel.queue(snd2)
                return True

        if busy and not queued:
            chunk = self._next_stereo_chunk()
            if chunk is not None and chunk.size > 0:
                snd = self._make_sound_from_array(chunk)
                self.channel.queue(snd)

        if self.mono is not None and self.pos < len(self.mono):
            return True
        return self.channel.get_busy()

    def is_playing(self) -> bool:
        return bool(self.channel and self.channel.get_busy())

    def stop(self):
        if self.channel:
            self.channel.stop()

    def close(self):
        self.stop()
        try:
            if pygame.mixer.get_init():
                pygame.mixer.quit()
        except Exception:
            pass
