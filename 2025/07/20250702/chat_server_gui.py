import socket_utils as net
import queue
import time
import tkinter as tk
from tkinter import scrolledtext, messagebox
import threading

HOST = 'localhost'
PORT = 12345

class ChatServerGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Chat Server")
        self.root.geometry("600x500")
        
        # GUI要素の作成
        self.create_widgets()
        
        # ソケット関連の変数
        self.server_socket = None
        self.conn = None
        self.send_queue = queue.Queue()
        self.receive_queue = queue.Queue()
        self.send_thread = None
        self.receive_thread = None
        self.is_connected = False
        
        # サーバー開始
        self.start_server()
        
        # 定期的にキューをチェック
        self.check_queue()
        
    def create_widgets(self):
        # メッセージ表示エリア
        self.chat_display = scrolledtext.ScrolledText(
            self.root, 
            wrap=tk.WORD, 
            width=70, 
            height=20,
            state=tk.DISABLED
        )
        self.chat_display.pack(padx=10, pady=10, fill=tk.BOTH, expand=True)
        
        # 入力フレーム
        input_frame = tk.Frame(self.root)
        input_frame.pack(fill=tk.X, padx=10, pady=(0, 10))
        
        # メッセージ入力欄
        self.message_entry = tk.Entry(input_frame, font=("Arial", 12))
        self.message_entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 5))
        self.message_entry.bind("<Return>", self.send_message)
        
        # 送信ボタン
        self.send_button = tk.Button(
            input_frame, 
            text="Send", 
            command=self.send_message,
            state=tk.DISABLED
        )
        self.send_button.pack(side=tk.RIGHT)
        
        # ステータス表示
        self.status_label = tk.Label(
            self.root, 
            text=f"Listening on {HOST}:{PORT}...", 
            relief=tk.SUNKEN, 
            anchor=tk.W
        )
        self.status_label.pack(fill=tk.X, side=tk.BOTTOM)
        
    def add_message(self, message, sender=""):
        """チャット表示エリアにメッセージを追加"""
        self.chat_display.config(state=tk.NORMAL)
        timestamp = time.strftime("%H:%M:%S")
        if sender:
            self.chat_display.insert(tk.END, f"[{timestamp}] {sender}: {message}\n")
        else:
            self.chat_display.insert(tk.END, f"[{timestamp}] {message}\n")
        self.chat_display.config(state=tk.DISABLED)
        self.chat_display.see(tk.END)
        
    def start_server(self):
        """サーバーを開始"""
        def server_thread():
            try:
                self.server_socket = net.create_server_socket(host=HOST, port=PORT)
                self.add_message(f"Server listening on {HOST}:{PORT}")
                
                # クライアントの接続を待機
                self.conn, addr = self.server_socket.accept()
                self.add_message(f"Connected by {addr}")
                self.status_label.config(text=f"Connected to {addr}")
                
                # 送受信スレッド開始
                self.send_thread = net.start_send_thread(self.conn, self.send_queue)
                self.receive_thread = net.start_receive_thread(self.conn, self.receive_queue)
                
                self.is_connected = True
                self.send_button.config(state=tk.NORMAL)
                self.message_entry.config(state=tk.NORMAL)
                
            except Exception as e:
                self.add_message(f"Server error: {str(e)}")
                
        # サーバーを別スレッドで開始
        threading.Thread(target=server_thread, daemon=True).start()
        
    def send_message(self, event=None):
        """メッセージを送信"""
        if not self.is_connected:
            return
            
        message = self.message_entry.get().strip()
        if not message:
            return
            
        if message.lower() in ['exit', 'quit']:
            self.disconnect()
            return
            
        # メッセージをキューに追加
        self.send_queue.put(message)
        
        # 自分のメッセージを表示
        self.add_message(message, "Server")
        
        # 入力欄をクリア
        self.message_entry.delete(0, tk.END)
        
    def check_queue(self):
        """受信キューを定期的にチェック"""
        try:
            while True:
                # ノンブロッキングでキューから取得
                received_message = self.receive_queue.get_nowait()
                if received_message is None:  # 終了シグナル
                    self.disconnect()
                    break
                self.add_message(received_message, "Client")
        except queue.Empty:
            # キューが空の場合は何もしない
            pass
            
        # 100ms後に再度チェック
        self.root.after(100, self.check_queue)
        
    def disconnect(self):
        """接続を切断"""
        if self.is_connected:
            self.send_queue.put(None)
            self.is_connected = False
            self.send_button.config(state=tk.DISABLED)
            self.message_entry.config(state=tk.DISABLED)
            self.add_message("Connection closed")
            self.status_label.config(text="Disconnected")
            
        if self.conn:
            self.conn.close()
        if self.server_socket:
            self.server_socket.close()
            
    def on_closing(self):
        """ウィンドウを閉じる時の処理"""
        self.disconnect()
        self.root.destroy()

def main():
    root = tk.Tk()
    app = ChatServerGUI(root)
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    root.mainloop()

if __name__ == "__main__":    
    main()