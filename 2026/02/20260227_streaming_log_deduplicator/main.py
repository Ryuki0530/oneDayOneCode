import sys
from collections import defaultdict

def main():
    W = int(sys.stdin.readline())
    
    last_accepted = {}
    
    event_counts = defaultdict(int)
    user_counts = defaultdict(int)
    total_count = 0
    
    for line in sys.stdin:
        parts = line.strip().split()
        timestamp = int(parts[0])
        user_id = parts[1]
        event_name = parts[2]
        
        key = (user_id, event_name)
        
        if key not in last_accepted:
            accept = True
        else:
            t_last = last_accepted[key]
            accept = (timestamp - t_last) > W
        
        if accept:
            last_accepted[key] = timestamp
            event_counts[event_name] += 1
            user_counts[user_id] += 1
            total_count += 1
    
    print(f"TOTAL {total_count}")
    
    sorted_events = sorted(event_counts.items(), key=lambda x: (-x[1], x[0]))
    for event_name, count in sorted_events:
        print(f"EVENT {event_name} {count}")
    
    sorted_users = sorted(user_counts.items(), key=lambda x: (-x[1], x[0]))
    for user_id, count in sorted_users:
        print(f"USER {user_id} {count}")

if __name__ == "__main__":
    main()