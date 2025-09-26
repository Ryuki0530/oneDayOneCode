#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
OneDayOneCode: PyQt「二窓」MP4プレイヤー
メインウィンドウ
"""

import sys
import argparse
from pathlib import Path
from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QMenuBar, QFileDialog, QMessageBox
)
from PyQt6.QtCore import Qt
from PyQt6.QtGui import QAction

from player_window import PlayerWindow


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.player_window = None  # 再生ウィンドウのインスタンス
        self.init_ui()
        
    def init_ui(self):
        """メインウィンドウのUI初期化"""
        self.setWindowTitle("MP4プレイヤー - メイン")
        self.setGeometry(100, 100, 400, 200)
        
        # メニューバーの作成
        self.create_menu_bar()
        
        print("メインウィンドウを初期化しました")
        
    def create_menu_bar(self):
        """メニューバーとファイルメニューの作成"""
        menubar = self.menuBar()
        
        # ファイルメニュー
        file_menu = menubar.addMenu("ファイル(&F)")
        
        # 「開く...」アクション
        open_action = QAction("開く...(&O)", self)
        open_action.setShortcut("Ctrl+O")
        open_action.setStatusTip("MP4ファイルを開く")
        open_action.triggered.connect(self.open_file)
        file_menu.addAction(open_action)
        
        # 区切り線
        file_menu.addSeparator()
        
        # 「終了」アクション
        exit_action = QAction("終了(&X)", self)
        exit_action.setShortcut("Ctrl+Q")
        exit_action.setStatusTip("アプリケーションを終了")
        exit_action.triggered.connect(self.close)
        file_menu.addAction(exit_action)
        
    def open_file(self):
        """ファイルダイアログでMP4ファイルを選択"""
        file_path, _ = QFileDialog.getOpenFileName(
            self,
            "動画を開く",
            "",
            "Video Files (*.mp4);;All Files (*)"
        )
        
        if file_path:
            print(f"選択されたファイル: {file_path}")
            self.load_video(file_path)
            
    def load_video(self, file_path):
        """動画ファイルを再生ウィンドウで読み込み・再生"""
        try:
            # ファイルの存在確認
            if not Path(file_path).exists():
                QMessageBox.critical(
                    self,
                    "ファイルエラー",
                    f"ファイルが見つかりません:\n{file_path}"
                )
                return
                
            # 再生ウィンドウが存在しない場合は新規作成
            if self.player_window is None:
                self.player_window = PlayerWindow()
                # 再生ウィンドウが閉じられた時の処理
                self.player_window.window_closed.connect(self.on_player_window_closed)
                
            # 再生ウィンドウを表示
            self.player_window.show()
            self.player_window.raise_()
            self.player_window.activateWindow()
            
            # 動画を読み込み・再生
            self.player_window.load_and_play_video(file_path)
            
            print(f"Loaded: {file_path}")
            
        except Exception as e:
            print(f"動画読み込みエラー: {e}")
            QMessageBox.critical(
                self,
                "読み込みエラー",
                f"動画の読み込みに失敗しました:\n{str(e)}"
            )
            
    def on_player_window_closed(self):
        """再生ウィンドウが閉じられた時の処理"""
        print("再生ウィンドウが閉じられました")
        self.player_window = None
        
    def closeEvent(self, event):
        """メインウィンドウが閉じられる時の処理"""
        # 再生ウィンドウも閉じる
        if self.player_window is not None:
            self.player_window.close()
            
        # アプリケーション全体を終了
        print("アプリケーションを終了します")
        event.accept()


def main():
    """メイン関数"""
    # コマンドライン引数の解析
    parser = argparse.ArgumentParser(description="PyQt MP4プレイヤー")
    parser.add_argument("--file", type=str, help="起動時に開くMP4ファイル")
    args = parser.parse_args()
    
    # アプリケーションの作成
    app = QApplication(sys.argv)
    app.setApplicationName("PyQt MP4プレイヤー")
    
    # メインウィンドウの作成・表示
    main_window = MainWindow()
    main_window.show()
    
    # コマンドライン引数で指定されたファイルがあれば開く
    if args.file:
        if Path(args.file).exists():
            main_window.load_video(args.file)
        else:
            print(f"指定されたファイルが見つかりません: {args.file}")
    
    # イベントループの開始
    sys.exit(app.exec())


if __name__ == "__main__":
    main()