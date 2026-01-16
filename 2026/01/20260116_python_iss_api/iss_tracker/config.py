# iss_tracker/config.py
# - べた書き定数をConfigクラスに集約して、将来の変更点を一箇所にした

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class AppConfig:
    # サーバ設定
    host: str = "127.0.0.1"
    port: int = 8000

    # ISS API
    iss_api: str = "https://api.wheretheiss.at/v1/satellites/25544"

    # 更新間隔
    update_ms: int = 300_000  # 5分
