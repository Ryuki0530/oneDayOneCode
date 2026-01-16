# main.py

from iss_tracker.config import AppConfig
from iss_tracker.build import build_web

def main() -> None:
    cfg = AppConfig()

    # HTMLをテンプレから生成
    build_web(cfg)

    # サーバ起動 + ブラウザ起動
    run_server(cfg)


if __name__ == "__main__":
    main()
