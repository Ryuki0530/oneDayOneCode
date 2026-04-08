n = int(input())
previous = None
count = 0

for _ in range(n):
    current = input()
    if current == previous:
        count += 1
    else:
        count = 1
        previous = current
    print(count)