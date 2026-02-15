import heapq

now = 0
store = {}
pq = []

def put(key, value, ttl):
    global now
    expires_at = now + ttl
    store[key] = (value, expires_at)
    heapq.heappush(pq, (expires_at, key))

def get(key):
    global now
    if key not in store:
        return "NULL"
    value, expires_at = store[key]
    if expires_at <= now:
        del store[key]
        return "NULL"
    return value

def sleep(t):
    global now
    now += t

def clean():
    global now
    while pq:
        expires_at, key = pq[0]
        if key not in store:
            heapq.heappop(pq)
            continue
        _, actual_expires = store[key]
        if actual_expires != expires_at:
            heapq.heappop(pq)
            continue
        if expires_at <= now:
            heapq.heappop(pq)
            del store[key]
        else:
            break

# Main
Q = int(input())
for _ in range(Q):
    cmd = input().split()
    
    if cmd[0] == "PUT":
        key, value, ttl = cmd[1], cmd[2], int(cmd[3])
        put(key, value, ttl)
    elif cmd[0] == "GET":
        key = cmd[1]
        print(get(key))
    elif cmd[0] == "SLEEP":
        t = int(cmd[1])
        sleep(t)
    elif cmd[0] == "CLEAN":
        clean()