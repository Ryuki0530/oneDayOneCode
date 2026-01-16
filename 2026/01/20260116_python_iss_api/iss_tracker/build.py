# iss_tracker/build.py
from __future__ import annotations
from pathlib import Path
from iss_tracker.config import AppConfig


def _load_template(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def _render(template: str, cfg: AppConfig) -> str:
    return (
        template.replace("{{ISS_API}}", cfg.iss_api)
        .replace("{{UPDATE_MS}}", str(cfg.update_ms))
    )


def build_web(cfg: AppConfig) -> None:
    cfg.web_out_dir.mkdir(exist_ok=True)

    template_path = cfg.web_src_dir / cfg.index_name
    template = _load_template(template_path)
    html = _render(template, cfg)

    cfg.index_out_path.write_text(html, encoding="utf-8")
