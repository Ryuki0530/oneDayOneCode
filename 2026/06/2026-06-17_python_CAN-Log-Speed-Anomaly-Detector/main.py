import csv
import sys


def load_can_log(filename):
    speed_data = []
    ignored_count = 0
    
    with open(filename, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            can_id = row["id"].strip()
            if can_id == "0x100":
                time = float(row["time"])
                speed = int(row["data"], 16)
                speed_data.append((time, speed))
            else:
                ignored_count += 1
    
    return speed_data, ignored_count


def detect_speed_alerts(speed_data, threshold=20):
    alerts = []
    
    for i in range(1, len(speed_data)):
        prev_time, prev_speed = speed_data[i - 1]
        curr_time, curr_speed = speed_data[i]
        
        time_delta = curr_time - prev_time
        speed_delta = curr_speed - prev_speed
        acceleration = speed_delta / time_delta
        
        if acceleration >= threshold or acceleration <= -threshold:
            alerts.append((curr_time, curr_speed, acceleration))
    
    return alerts


def analyze_speed_stats(speed_data):
    if not speed_data:
        return None
    
    speeds = [speed for _, speed in speed_data]
    max_speed = max(speeds)
    min_speed = min(speeds)
    avg_speed = sum(speeds) / len(speeds)
    
    return {
        "max": max_speed,
        "min": min_speed,
        "avg": avg_speed
    }


if __name__ == "__main__":
    threshold = 20
    if len(sys.argv) > 1:
        try:
            threshold = float(sys.argv[1])
        except ValueError:
            print(f"Warning: Invalid threshold value. Using default: {threshold}")
    
    speed_data, ignored_count = load_can_log("can_log.csv")
    alerts = detect_speed_alerts(speed_data, threshold)
    stats = analyze_speed_stats(speed_data)
    
    print(f"[INFO] 0x100以外のID件数: {ignored_count}")
    
    if stats:
        print(f"[INFO] 最大速度: {stats['max']}km/h")
        print(f"[INFO] 最小速度: {stats['min']}km/h")
        print(f"[INFO] 平均速度: {stats['avg']:.2f}km/h")
    
    print()
    
    if alerts:
        print(f"[警告] 異常を{len(alerts)}件検出しました:")
        for time, speed, acceleration in alerts:
            print(f"[ALERT] time={time}s speed={speed}km/h delta={acceleration:.1f}km/h/s")
    else:
        print("[OK] 異常は検出されませんでした")
