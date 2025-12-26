import sys
import sys

def main():
    input = sys.stdin.readline

    N, D = map(int, sys.stdin.readline().split())
    times = list(map(int, sys.stdin.readline().split()))
    times.sort()

    max_active = 0
    l = 0
    for r in range(N):
        while times[r] - times[l] >= D:
            l += 1
        max_active = max(max_active, r - l + 1)
    print(max_active)

if __name__ == "__main__":
    main()