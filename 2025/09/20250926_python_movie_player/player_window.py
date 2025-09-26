#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
OneDayOneCode: PyQt「二窓」MP4プレイヤー
再生ウィンドウ
"""

from PyQt6.QtWidgets import QMainWindow, QVBoxLayout, QWidget, QMessageBox
from PyQt6.QtMultimedia import QMediaPlayer, QAudioOutput
from PyQt6.QtMultimediaWidgets import QVideoWidget
from PyQt6.QtCore import Qt, pyqtSignal, QUrl
from PyQt6.QtGui import QKeySequence


class PlayerWindow(QMainWindow):
    # 再生ウィンドウが閉じられた時のシグナル
    window_closed = pyqtSignal()
    
    def __init__(self):
        super().__init__()
        self.media_player = None
        self.audio_output = None
        self.video_widget = None
        self.current_file = None
        self.init_ui()
        self.setup_media_player()
        
    def init_ui(self):
        """再生ウィンドウのUI初期化"""
        self.setWindowTitle("動画再生ウィンドウ")
        self.setGeometry(200, 200, 800, 600)
        
        # 中央ウィジェットの設定
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        # レイアウト
        layout = QVBoxLayout()
        central_widget.setLayout(layout)
        
        # 動画表示ウィジェット
        self.video_widget = QVideoWidget()
        layout.addWidget(self.video_widget)
        
        # キーボードフォーカスを受け取るようにする
        self.setFocusPolicy(Qt.FocusPolicy.StrongFocus)
        
        print("再生ウィンドウを初期化しました")
        
    def setup_media_player(self):
        """メディアプレイヤーの設定"""
        # オーディオ出力の設定
        self.audio_output = QAudioOutput()
        self.audio_output.setVolume(0.7)  # 音量70%
        
        # メディアプレイヤーの設定
        self.media_player = QMediaPlayer()
        self.media_player.setAudioOutput(self.audio_output)
        self.media_player.setVideoOutput(self.video_widget)
        
        # シグナルの接続
        self.media_player.errorChanged.connect(self.on_media_error)
        self.media_player.playbackStateChanged.connect(self.on_playback_state_changed)
        self.media_player.positionChanged.connect(self.on_position_changed)
        self.media_player.durationChanged.connect(self.on_duration_changed)
        
        print("メディアプレイヤーを設定しました")
        
    def load_and_play_video(self, file_path):
        """動画ファイルを読み込み、自動再生"""
        try:
            self.current_file = file_path
            
            # ファイルURLの設定
            file_url = QUrl.fromLocalFile(file_path)
            self.media_player.setSource(file_url)
            
            # 自動再生
            self.media_player.play()
            
            print(f"動画を読み込み開始: {file_path}")
            
        except Exception as e:
            print(f"動画読み込みエラー: {e}")
            self.show_error_message("読み込みエラー", f"動画の読み込みに失敗しました:\n{str(e)}")
            
    def keyPressEvent(self, event):
        """キーボードイベントの処理"""
        if event.key() == Qt.Key.Key_Space:
            # スペースキー: 再生/一時停止のトグル
            self.toggle_playback()
            event.accept()
        elif event.key() == Qt.Key.Key_Escape:
            # Escキー: ウィンドウを閉じる
            self.close()
            event.accept()
        else:
            # その他のキーは親クラスに委譲
            super().keyPressEvent(event)
            
    def toggle_playback(self):
        """再生/一時停止のトグル"""
        if self.media_player.playbackState() == QMediaPlayer.PlaybackState.PlayingState:
            self.media_player.pause()
            print("動画を一時停止しました")
        else:
            self.media_player.play()
            print("動画を再生しました")
            
    def on_media_error(self, error):
        """メディアエラーの処理"""
        if error != QMediaPlayer.Error.NoError:
            error_string = self.media_player.errorString()
            print(f"メディアエラー: {error_string}")
            
            # エラーの種類に応じたメッセージ
            if error == QMediaPlayer.Error.ResourceError:
                message = f"ファイルの読み込みに失敗しました。\nファイルが破損しているか、対応していないフォーマットの可能性があります。\n\nエラー詳細: {error_string}"
            elif error == QMediaPlayer.Error.FormatError:
                message = f"対応していないコーデックです。\nH.264/AAC形式のMP4ファイルをお試しください。\n\nエラー詳細: {error_string}"
            elif error == QMediaPlayer.Error.NetworkError:
                message = f"ネットワークエラーが発生しました。\n\nエラー詳細: {error_string}"
            else:
                message = f"再生エラーが発生しました。\n\nエラー詳細: {error_string}"
                
            self.show_error_message("再生エラー", message)
            
    def on_playback_state_changed(self, state):
        """再生状態変更の処理"""
        if state == QMediaPlayer.PlaybackState.PlayingState:
            print("再生状態: 再生中")
        elif state == QMediaPlayer.PlaybackState.PausedState:
            print("再生状態: 一時停止")
        elif state == QMediaPlayer.PlaybackState.StoppedState:
            print("再生状態: 停止")
            
    def on_position_changed(self, position):
        """再生位置変更の処理（デバッグ用）"""
        # 10秒ごとにログ出力（あまり頻繁だとログが多くなるため）
        if position % 10000 == 0:  # positionはミリ秒単位
            print(f"再生位置: {position // 1000}秒")
            
    def on_duration_changed(self, duration):
        """再生時間変更の処理"""
        if duration > 0:
            print(f"動画の長さ: {duration // 1000}秒")
            
    def show_error_message(self, title, message):
        """エラーメッセージダイアログの表示"""
        msg_box = QMessageBox(self)
        msg_box.setIcon(QMessageBox.Icon.Critical)
        msg_box.setWindowTitle(title)
        msg_box.setText(message)
        msg_box.exec()
        
    def closeEvent(self, event):
        """ウィンドウが閉じられる時の処理"""
        # 再生停止とリソース解放
        if self.media_player:
            self.media_player.stop()
            
        # メインウィンドウに通知
        self.window_closed.emit()
        
        print("再生ウィンドウを閉じます")
        event.accept()
        
    def showEvent(self, event):
        """ウィンドウが表示される時の処理"""
        super().showEvent(event)
        # フォーカスを設定してキーボードイベントを受け取れるようにする
        self.setFocus()