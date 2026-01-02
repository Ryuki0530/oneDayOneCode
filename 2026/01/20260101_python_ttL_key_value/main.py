import sys

input = sys.stdin.readline

Q = int(input())
store = {}

for _ in range(Q):
    parts = input().split()
    if parts[0] == 'SET':
        _, key, value, ttl, now = parts
        store[key] = (int(value), int(now) + int(ttl))
    elif parts[0] == 'GET':
        _, key, now = parts
        now = int(now)
        if key in store:
            value, expire = store[key]
            if expire > now:
                print(value)
            else:
                del store[key]
                print(-1)
        else:
            print(-1)
    elif parts[0] == 'DEL':
        _, key = parts
        store.pop(key, None)