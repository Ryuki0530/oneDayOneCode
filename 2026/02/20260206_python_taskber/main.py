import threading
import json
import os
from pathlib import Path
from datetime import datetime
import logging
from pystray import Icon, Menu, MenuItem
from PIL import Image, ImageDraw

# main.py


# ===== ロギング設定 =====
logging.basicConfig(
    filename="app.log",
    level=logging.INFO,
    format="%(asctime)s - %(message)s"
)

# ===== State 管理 =====
STATE_FILE = "state.json"

def load_state() -> bool:
    """保存された状態を読み込む（デフォルト: False）"""
    if os.path.exists(STATE_FILE):
        try:
            with open(STATE_FILE, "r") as f:
                data = json.load(f)
                return data.get("is_on", False)
        except Exception as e:
            logging.error(f"Failed to load state: {e}")
    return False

def save_state(is_on: bool) -> None:
    """状態をファイルに保存"""
    try:
        with open(STATE_FILE, "w") as f:
            json.dump({"is_on": is_on, "updated_at": datetime.now().isoformat()}, f)
    except Exception as e:
        logging.error(f"Failed to save state: {e}")

# ===== アイコン画像生成 =====
def create_icon_image(is_on: bool, size: int = 64) -> Image.Image:
    """ON/OFF状態を示すアイコンを生成"""
    try:
        img = Image.new("RGBA", (size, size), color=(40, 40, 40, 255))
        draw = ImageDraw.Draw(img)
        
        # ON=緑、OFF=赤
        color = (0, 200, 0, 255) if is_on else (200, 0, 0, 255)
        
        # 中央に円を描画
        margin = 8
        draw.ellipse(
            [(margin, margin), (size - margin, size - margin)],
            fill=color,
            outline=(255, 255, 255, 255),
            width=2
        )
        
        logging.info(f"Icon image created: size={img.size}, mode={img.mode}")
        return img
    except Exception as e:
        logging.error(f"Failed to create icon image: {e}")
        raise

# ===== トレイアプリ本体 =====
class TrayApp:
    def __init__(self):
        self.is_on = load_state()
        self.icon = None
        logging.info("App started")
    
    def toggle_state(self, icon, item):
        """状態を切り替え"""
        self.is_on = not self.is_on
        save_state(self.is_on)
        logging.info(f"State toggled: {self.is_on}")
        
        # アイコンを更新
        self.update_icon()
        
        # 通知を表示
        status_text = "ON" if self.is_on else "OFF"
        icon.notify(f"State is now: {status_text}", "Status Changed")
    
    def show_status(self, icon, item):
        """現在の状態を通知で表示"""
        status_text = "ON" if self.is_on else "OFF"
        icon.notify(f"Current state: {status_text}", "Status Info")
        logging.info(f"Status displayed: {status_text}")
    
    def exit_app(self, icon, item):
        """アプリを終了"""
        logging.info("App exiting")
        icon.stop()
    
    def update_icon(self):
        """アイコン画像とツールチップを更新"""
        if self.icon:
            self.icon.icon = create_icon_image(self.is_on)
            self.icon.title = f"Tray App - {'ON' if self.is_on else 'OFF'}"
    
    def setup_icon(self):
        """トレイアイコンをセットアップ"""
        menu = Menu(
            MenuItem("Toggle", self.toggle_state),
            MenuItem("Show status", self.show_status),
            MenuItem("Exit", self.exit_app),
        )
        
        icon_image = create_icon_image(self.is_on)
        logging.info(f"Setting up icon with image: {icon_image}")
        
        self.icon = Icon(
            name="tray_app",
            icon=icon_image,
            title=f"Tray App - {'ON' if self.is_on else 'OFF'}",
            menu=menu
        )
    
    def run(self):
        """アプリを実行"""
        self.setup_icon()
        self.icon.run()

if __name__ == "__main__":
    app = TrayApp()
    app.run()