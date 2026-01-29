import sys

n = int(sys.stdin.readline())
result = []
prev = None
cnt = 0

for _ in range(n):
    s = sys.stdin.readline().rstrip('\n')
    
    if s == prev:
        cnt += 1
    else:
        if prev is not None:
            result.append(f"{cnt} {prev}")
        prev = s
        cnt = 1

if prev is not None:
    result.append(f"{cnt} {prev}")

print("\n".join(result))