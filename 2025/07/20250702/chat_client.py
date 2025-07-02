import socket_utils as net
import queue
import time
HOST = 'localhost'
PORT = 12345

def main():
    client_socket = net.create_client_socket(host=HOST, port=PORT)
    print(f"[Client] Connected to {HOST}:{PORT}")

    send_queue = queue.Queue()
    receive_queue = queue.Queue()

    #送受信スレッド作成
    
    send_thread = net.start_send_thread(client_socket, send_queue)
    receive_thread = net.start_receive_thread(client_socket, receive_queue)

    try:
        while True:
            # 受信キューにメッセージがあるかチェック
            try:
                # ノンブロッキングでキューから取得
                received_message = receive_queue.get_nowait()
                if received_message is None:  # 終了シグナル
                    break
                print(f"[Server] {received_message}")
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
            except EOFError:
                # 入力がない場合は短時間待機
                time.sleep(0.1)
    except KeyboardInterrupt:
        send_queue.put(None)
    finally:
        print("[Client] Shutting down...")
        client_socket.close()
        
if __name__ == "__main__":
    main()