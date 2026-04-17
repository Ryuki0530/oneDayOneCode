from collections import defaultdict

status_counts = defaultdict(int)

while True:
    line = input()
    if line == "END":
        break
    
    parts = line.split()
    status_code = int(parts[1])
    status_counts[status_code] += 1

for status_code in sorted(status_counts.keys()):
    print(f"{status_code} {status_counts[status_code]}")