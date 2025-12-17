import sys

def main():
    data = sys.stdin.read().strip().splitlines()
    if not data:
        return
    n = int(data[0])
    timestamps = []
    values = []
    for line in data[1:n+1]:
        ts, val = line.split()
        timestamps.append(ts)
        values.append(int(val))

    best_start = best_end = 0
    best_len = 0
    curr_start = 0
    curr_len = 1

    for i in range(1, n):
        if values[i] > values[i - 1]:
            curr_len += 1
        else:
            if curr_len > best_len or (curr_len == best_len and curr_start < best_start):
                best_start = curr_start
                best_end = i - 1
                best_len = curr_len
            curr_start = i
            curr_len = 1

    if curr_len > best_len or (curr_len == best_len and curr_start < best_start):
        best_start = curr_start
        best_end = n - 1
        best_len = curr_len

    print(f"{timestamps[best_start]} {timestamps[best_end]} {best_len}")

if __name__ == "__main__":
    main()