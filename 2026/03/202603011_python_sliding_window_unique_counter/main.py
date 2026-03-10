import sys
from collections import defaultdict

def main():
    input = sys.stdin.readline
    N, K = map(int, input().split())
    A = list(map(int, input().split()))

    count = defaultdict(int)
    distinct = 0
    ans = []

    for i in range(K):
        if count[A[i]] == 0:
            distinct += 1
        count[A[i]] += 1

    ans.append(str(distinct))

    for i in range(K, N):
        left = A[i - K]
        count[left] -= 1
        if count[left] == 0:
            distinct -= 1

        right = A[i]
        if count[right] == 0:
            distinct += 1
        count[right] += 1

        ans.append(str(distinct))

    print("\n".join(ans))

if __name__ == "__main__":
    main()