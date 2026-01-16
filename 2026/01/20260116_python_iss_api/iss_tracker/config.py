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

    # Web関連パス
    web_src_dir: Path = Path(__file__).resolve().parents[1] / "web"
    web_out_dir: Path = Path(__file__).resolve().parents[1] / "iss_web"

    # 出力HTMLファイル名
    index_name: str = "index.html"

    # 更新間隔
    update_ms: int = 300_000


    @property
    def index_out_path(self) -> Path:
        return self.web_out_dir / self.index_name

    @property
    def url(self) -> str:
        return f"http://{self.host}:{self.port}/{self.index_name}"