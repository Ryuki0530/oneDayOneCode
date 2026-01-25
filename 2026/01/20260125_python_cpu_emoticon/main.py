import psutil
import time
import argparse
from datetime import datetime

def get_emoticon_and_comment(cpu, mem, disk, net_speed):
    
    # 断末魔
    if disk >= 95:
        return "(；ﾟДﾟ)", "断末魔"
    
    # 息切れ
    if mem >= 85:
        return "(；´Д｀)", "息切れ"
    
    # 忙しい（CPU高負荷）
    if cpu >= 80:
        return "(；`皿´)", "忙しい"
    
    # 通信祭り
    if net_speed >= 1024 * 1024:  # 1MB/s以上
        return "＼(＾o＾)／", "通信祭り"
    
    # 平常
    if cpu < 40 and mem < 60 and disk < 80:
        return "(・ω・)", "平常運転"
    
    # その他
    return "(´・ω・`)", "稼働中"


def format_bytes(bytes_val):
    for unit in ["B", "KB", "MB", "GB"]:
        if bytes_val < 1024:
            return f"{bytes_val:.1f}{unit}"
        bytes_val /= 1024
    return f"{bytes_val:.1f}TB"


def main():
    parser = argparse.ArgumentParser(description="CPU の気分を顔文字で表示")
    parser.add_argument("--interval", type=float, default=1.0, help="更新間隔（秒）")
    args = parser.parse_args()
    
    psutil.cpu_percent(interval=None)
    
    prev_net = psutil.net_io_counters()
    
    try:
        while True:
            now = datetime.now().strftime("%H:%M:%S")
            
            cpu = psutil.cpu_percent(interval=None)
            mem = psutil.virtual_memory().percent
            disk = psutil.disk_usage("C:\\").percent
            
            time.sleep(args.interval)
            cur_net = psutil.net_io_counters()
            net_speed = cur_net.bytes_sent + cur_net.bytes_recv - prev_net.bytes_sent - prev_net.bytes_recv
            prev_net = cur_net
            
            emoticon, comment = get_emoticon_and_comment(cpu, mem, disk, net_speed)
            
            output = f"{now} CPU {cpu:5.1f}%     MEM {mem:5.1f}%     DISK {disk:5.1f}%     NET {format_bytes(net_speed)}/s       {emoticon} {comment}"
            print("\r" + output, end="", flush=True)
    
    except KeyboardInterrupt:
        print()


if __name__ == "__main__":
    main()