from typing import List, Dict, Any
import json
from datetime import datetime
import os

class WindowEvent:
    def __init__(self, hwnd: int, title: str, pid: int, event_type: str, timestamp: str = None):
        self.hwnd = hwnd
        self.title = title
        self.pid = pid
        self.event_type = event_type  # "start" or "end"
        self.timestamp = timestamp or datetime.now().isoformat()

    def to_dict(self):
        return {
            "hwnd": self.hwnd,
            "title": self.title,
            "pid": self.pid,
            "event_type": self.event_type,
            "timestamp": self.timestamp
        }

class WindowWatcherModel:
    def __init__(self):
        self.current_windows: Dict[int, Dict[str, Any]] = {}  # hwnd -> window info
        self.event_log: List[Dict[str, Any]] = []

    def initialize(self, windows):
        """
        windows: 初期ウィンドウ情報リスト（上位モジュールから渡される）
        """
        self.current_windows.clear()
        self.event_log.clear()
        for win in windows:
            self.current_windows[win.hwnd] = win.__dict__
            event = WindowEvent(win.hwnd, win.title, win.pid, "start")
            self.event_log.append(event.to_dict())

    def update(self, windows):
        """
        windows: 新しいウィンドウ情報リスト（上位モジュールから渡される）
        """
        now_hwnds = set(win.hwnd for win in windows)
        prev_hwnds = set(self.current_windows.keys())

        # 新規起動ウィンドウ
        for win in windows:
            if win.hwnd not in self.current_windows:
                self.current_windows[win.hwnd] = win.__dict__
                event = WindowEvent(win.hwnd, win.title, win.pid, "start")
                self.event_log.append(event.to_dict())

        # 終了したウィンドウ
        for hwnd in prev_hwnds - now_hwnds:
            win_info = self.current_windows[hwnd]
            event = WindowEvent(hwnd, win_info["title"], win_info["pid"], "end")
            self.event_log.append(event.to_dict())
            del self.current_windows[hwnd]


        #debug
        # print(f"Current windows: {len(self.current_windows)}, Events logged: {len(self.event_log)}")
        # print(f"Current windows: {self.current_windows}")
        # print(f"Event log (last 5): {self.event_log[-5:]}")
        

    def save_log(self, filepath: str):
        os.makedirs(os.path.dirname(filepath), exist_ok=True)
        with open(filepath, "w", encoding="utf-8") as f:
            json.dump(self.event_log, f, ensure_ascii=False, indent=2)