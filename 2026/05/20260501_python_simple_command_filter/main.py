n = int(input())
names = set()

for _ in range(n):
    command = input().split()
    action = command[0]
    name = command[1]
    
    if action == "ADD":
        names.add(name)
    elif action == "DEL":
        names.discard(name)
    elif action == "CHECK":
        if name in names:
            print("YES")
        else:
            print("NO")
