import sys

tokens = sys.stdin.read().split()
if not tokens:
    sys.exit()

counts = []
current = tokens[0]
streak = 1

for token in tokens[1:]:
    if token == current:
        streak += 1
    else:
        counts.append(str(streak))
        current = token
        streak = 1

counts.append(str(streak))
print(" ".join(counts))