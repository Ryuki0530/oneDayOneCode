import sys

user_items = {}

for line in sys.stdin:
    parts = line.strip().split()
    if len(parts) == 2:
        user_id, item_id = parts
        if user_id not in user_items:
            user_items[user_id] = set()
        
        user_items[user_id].add(item_id)

for user_id in sorted(user_items.keys()):
    count = len(user_items[user_id])
    print(f"{user_id} {count}")