import socket as net
import threading
import queue

def create_server_socket(host = "localhost", port = 10000,backlog = 1,buffer_size = 1024):
    """
    ソケット通信を行うサーバー側の関数
    引数：
        host :接続先情報(IPアドレス、ホスト名)
        port :ポート番号
        backlog :接続待ち数
        buffer_size :受信バッファのサイズ
    返り値:
        s :接続されたソケットオブジェクト
    """
    s = net.socket(net.AF_INET, net.SOCK_STREAM)
    s.bind((host, port))
    s.listen(backlog)
    return s


def create_client_socket(host = "localhost", port = 10000):
    """
    ソケット通信を行うクライアント側の関数
    引数：
        host :接続先情報(IPアドレス、ホスト名)
        port :ポート番号
    返り値:
        s :接続されたソケットオブジェクト
    """
    print(f"[Client] Creating socket to connect to {host}:{port} ...")
    s = net.socket(net.AF_INET, net.SOCK_STREAM)
    print(f"[Client] Connecting to {host}:{port} ...")
    s.connect((host, port))
    return s

def send_str(sock: net.socket, message: str):
    """
    ソケットを通じてメッセージを送信する関数
    引数：
        sock :ソケットオブジェクト
        message :送信するメッセージ
    """
    sock.sendall(message.encode('utf-8'))
    print(f"You: {message}")

def start_send_thread(sock: net.socket, send_queue: queue.Queue):
    """
    送信用のスレッドを開始する関数
    引数：
        sock :ソケットオブジェクト
        send_queue :送信メッセージを格納するキュー
        on_server :サーバー側かどうかのフラグ
    """
    def send_loop():
        while True:
            message = send_queue.get()
            if message is None:
                break
            send_str(sock, message)

    thread = threading.Thread(target=send_loop)
    thread.start()
    return thread

def start_receive_thread(sock: net.socket, receive_queue: queue.Queue):
    """
    受信用のスレッドを開始する関数
    引数：
        sock :ソケットオブジェクト
        receive_queue :受信メッセージを格納するキュー
    """
    def receive_loop():
        while True:
            data = sock.recv(1024)
            if not data:
                break
            message = data.decode('utf-8')
            
            #キューで受信メッセージを管理する場合
            receive_queue.put(message)
            # 直接受信メッセージを表示する場合
            # print(f"Received: {message}")
    
    thread = threading.Thread(target=receive_loop)
    thread.start()
    return thread