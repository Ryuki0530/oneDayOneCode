from collections import defaultdict

W = int(input())

last_occurrence = {}
event_count = defaultdict(int)
user_count = defaultdict(int)
total_count = 0

while True:
    try:
        line = input().strip()
        if not line:
            break
        parts = line.split()
        timestamp = int(parts[0])
        user = parts[1]
        event = parts[2]
        
        key = (user, event)
        
        if key in last_occurrence and timestamp - last_occurrence[key] <= W:
            continue
        
        last_occurrence[key] = timestamp
        event_count[event] += 1
        user_count[user] += 1
        total_count += 1
    except EOFError:
        break

print(f"TOTAL {total_count}")

sorted_events = sorted(event_count.items(), key=lambda x: (-x[1], x[0]))
for event, count in sorted_events:
    print(f"EVENT {event} {count}")

sorted_users = sorted(user_count.items(), key=lambda x: (-x[1], x[0]))
for user, count in sorted_users:
    print(f"USER {user} {count}")