import sys
from collections import defaultdict

user_count = defaultdict(int)
action_count = defaultdict(int)
total_count = 0

for line in sys.stdin:
    line = line.strip()
    
    if not line:
        continue
    
    parts = line.split()
    if len(parts) >= 2:
        user = parts[0]
        action = parts[1]
        
        user_count[user] += 1
        action_count[action] += 1
        total_count += 1

print(f"TOTAL {total_count}")

for user in sorted(user_count.keys()):
    print(f"{user} {user_count[user]}")

print()
for action in sorted(action_count.keys()):
    print(f"{action} {action_count[action]}")