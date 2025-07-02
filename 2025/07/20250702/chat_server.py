import socket_utils as net
import queue
import time
HOST = 'localhost'
PORT = 12345

def main():
    server_socket = net.create_server_socket(host=HOST, port=PORT)
    print(f"[Server] Listening on {HOST}:{PORT}")
    conn, addr = server_socket.accept()
    print(f"[Server] Connected by {addr}")

    send_queue = queue.Queue()
    receive_queue = queue.Queue()

    #送受信スレッド作成
    send_thread = net.start_send_thread(conn, send_queue)
    receive_thread = net.start_receive_thread(conn, receive_queue)

    try:
        while True:
            # 受信キューにメッセージがあるかチェック
            try:
                # ノンブロッキングでキューから取得
                received_message = receive_queue.get_nowait()
                if received_message is None:  # 終了シグナル
                    break
                print(f"[Client] {received_message}")
            except queue.Empty:
                # キューが空の場合は何もしない
                pass
            # ユーザー入力をチェック（ノンブロッキング）
            try:
                user_input = input()
                if user_input.lower() in ['exit', 'quit']:
                    send_queue.put(None)
                    break
                send_queue.put(user_input)
            except:
                # 入力がない場合は短時間待機
                time.sleep(0.1)
                
    except KeyboardInterrupt:
        send_queue.put(None)
    finally:
        print("[Server] Shutting down...")
        conn.close()
        server_socket.close()

if __name__ == "__main__":    
    main()