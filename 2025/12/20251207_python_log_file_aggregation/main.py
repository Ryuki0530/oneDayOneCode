from collections import Counter

paths = []
with open('access.log', 'r') as f:
    for line in f:
        parts = line.strip().split()
        if len(parts) >= 3:
            path = parts[2]
            paths.append(path)

if paths:
    counter = Counter(paths)
    most_common_path, count = counter.most_common(1)[0]
    print(f"Most accessed: {most_common_path} ({count} times)")