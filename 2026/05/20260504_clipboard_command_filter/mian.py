n = int(input())
for _ in range(n):
    line = input()
    kind, command = line.split(" ", 1)
    if kind == "RUN":
        print(command)
