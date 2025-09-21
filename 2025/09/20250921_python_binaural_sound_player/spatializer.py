# spatializer.py
from __future__ import annotations
import math
from dataclasses import dataclass
import numpy as np

Int16 = np.int16
Float32 = np.float32

@dataclass
class Pose:
    azimuth_deg: float = 0.0   # -180..180
    distance_m: float = 1.0    # 0.2..3.0

class Spatializer:
    """
    モノラル → ステレオ（ILD + ITD + 距離減衰 + optional LPF）
    """
    def __init__(self, sample_rate: int, k_atten: float = 0.5, use_lpf: bool = False):
        self.sample_rate = int(sample_rate)
        self.k_atten = float(k_atten)
        self.use_lpf = bool(use_lpf)
        self.pose = Pose()
        # LPF状態（片耳ずつ）
        self._lpf_state_L = 0.0
        self._lpf_state_R = 0.0

        # 定数：頭部幅 d[m], 音速 c[m/s]
        self._d = 0.18
        self._c = 343.0
        # 安全マージン：最大ITDサンプル数（約0.52ms）
        self._max_itd_samps = int(round(0.00052 * self.sample_rate))

    def set_pose(self, azimuth_deg: float, distance_m: float):
        # 正規化＆クリップ
        az = ((azimuth_deg + 180.0) % 360.0) - 180.0
        r = max(0.2, min(3.0, float(distance_m)))
        self.pose = Pose(azimuth_deg=az, distance_m=r)

    def current_itd_samples(self) -> int:
        """可視化/デバッグ用の現在ITDサンプル数"""
        theta = math.radians(self.pose.azimuth_deg)
        dt = (self._d / self._c) * math.sin(theta)
        N = int(round(dt * self.sample_rate))
        # 念のためクリップ
        if N > self._max_itd_samps: N = self._max_itd_samps
        if N < -self._max_itd_samps: N = -self._max_itd_samps
        return N

    def process(self, mono_chunk: np.ndarray) -> np.ndarray:
        """
        mono_chunk: shape (N,), dtype=int16
        return: shape (N, 2), dtype=int16（C連続）
        """
        if mono_chunk.size == 0:
            return np.zeros((0, 2), dtype=Int16)

        # float化（-1..1）
        x = mono_chunk.astype(Float32) / 32768.0

        # 距離減衰ゲイン g(r) = 1 / (1 + k r^2)
        r = self.pose.distance_m
        g = 1.0 / (1.0 + self.k_atten * (r * r))
        g = max(0.15, min(1.0, g))

        # ILD（等電力パン風）
        theta = math.radians(self.pose.azimuth_deg)
        p = math.sin(theta)  # -1..1
        L_gain = g * math.sqrt((1.0 - p) * 0.5)
        R_gain = g * math.sqrt((1.0 + p) * 0.5)

        left = x * L_gain
        right = x * R_gain

        # ITD（片耳ディレイ）
        N = self.current_itd_samples()
        if N > 0:
            # 右遅延：先頭に0をN、末尾を切り落とし
            if N >= len(right):
                right = np.zeros_like(right)
            else:
                right = np.concatenate([np.zeros(N, dtype=Float32), right[:-N]])
        elif N < 0:
            # 左遅延：左を|N|遅らせる
            M = -N
            if M >= len(left):
                left = np.zeros_like(left)
            else:
                left = np.concatenate([np.zeros(M, dtype=Float32), left[:-M]])

        # Optional: 距離依存の軽いLPF（一次IIR）
        if self.use_lpf:
            # 0.2m → alpha ~ 0.6、3.0m → alpha ~ 0.15 くらいに線形に
            alpha = 0.6 - (r - 0.2) * (0.45 / (3.0 - 0.2))
            alpha = max(0.12, min(0.7, alpha))
            self._lpf_state_L = self._lpf_iir_inplace(left, self._lpf_state_L, alpha)
            self._lpf_state_R = self._lpf_iir_inplace(right, self._lpf_state_R, alpha)

        # ステレオ化＆int16へ（C連続を保証）
        stereo = np.stack([left, right], axis=1)
        stereo = np.clip(stereo * 32767.0, -32768.0, 32767.0).astype(Int16)
        stereo = np.ascontiguousarray(stereo)
        return stereo

    @staticmethod
    def _lpf_iir_inplace(x: np.ndarray, state: float, alpha: float) -> float:
        # y[n] = (1-alpha)*y[n-1] + alpha*x[n]
        a = float(alpha)
        b = 1.0 - a
        y_prev = float(state)
        for i in range(x.shape[0]):
            y_prev = b * y_prev + a * float(x[i])
            x[i] = y_prev
        return y_prev
