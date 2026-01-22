N, T = map(int, input().split())
last_execution = {}

for _ in range(N):
    time, key = input().split()
    time = int(time)
    
    if key not in last_execution or time - last_execution[key] >= T:
        print(time, key)
        last_execution[key] = time