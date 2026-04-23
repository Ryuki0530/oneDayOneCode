s = input().strip()

current = s[0]
count = 1

for ch in s[1:]:
    if ch == current:
        count += 1
    else:
        print(f"{current}:{count}")
        current = ch
        count = 1

print(f"{current}:{count}")