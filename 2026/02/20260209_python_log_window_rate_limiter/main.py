from collections import defaultdict, deque

class RateLimiter:
    def __init__(self, limit: int, window_sec: int):
        self.limit = limit
        self.window_sec = window_sec
        self.user_logs = defaultdict(deque)
    
    def allow(self, user_id: str, now: int) -> bool:
        logs = self.user_logs[user_id]
        
        while logs and logs[0] <= now - self.window_sec:
            logs.popleft()
        
        if len(logs) < self.limit:
            logs.append(now)
            return True
        else:
            return False

limit, window_sec = map(int, input().split())
q = int(input())
limiter = RateLimiter(limit, window_sec)

for _ in range(q):
    now, user_id = input().split()
    now = int(now)
    result = limiter.allow(user_id, now)
    print(1 if result else 0)