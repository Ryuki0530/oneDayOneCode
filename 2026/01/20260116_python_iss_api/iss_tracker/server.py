# iss_tracker/server.py
from __future__ import annotations
import http.server
import socketserver
import threading
import webbrowser
from pathlib import Path
from iss_tracker.config import AppConfig


def serve_directory(root: Path, host: str, port: int) -> None:
    class Handler(http.server.SimpleHTTPRequestHandler):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, directory=str(root), **kwargs)

        def log_message(self, format, *args):
            pass

    with socketserver.TCPServer((host, port), Handler) as httpd:
        httpd.serve_forever()


def run_server(cfg: AppConfig) -> None:
    t = threading.Thread(
        target=serve_directory,
        args=(cfg.web_out_dir, cfg.host, cfg.port),
        daemon=True,
    )
    t.start()

    webbrowser.open(cfg.url)

    print("ISS Tracker is running.")
    print(f"Open: {cfg.url}")
    print("Press Ctrl+C to stop.")

    try:
        t.join()
    except KeyboardInterrupt:
        print("\nStopped.")
