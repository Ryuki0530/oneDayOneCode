# main.py

import sys


class BIT:
    def __init__(self, n: int):
        self.n = n
        self.bit = [0] * (n + 1)

    def add(self, idx: int, delta: int) -> None:
        i = idx + 1
        while i <= self.n:
            self.bit[i] += delta
            i += i & -i

    def sum(self, idx: int) -> int:
        i = idx + 1
        s = 0
        while i > 0:
            s += self.bit[i]
            i -= i & -i
        return s


def main() -> None:
    input = sys.stdin.readline
    N, Q = map(int, input().split())
    bit = BIT(N + 1)

    for _ in range(Q):
        op = input().split()
        if op[0] == "ADD":
            l = int(op[1])
            r = int(op[2])
            x = int(op[3])
            bit.add(l, x)
            bit.add(r, -x)
        else:  # GET
            i = int(op[1])
            print(bit.sum(i))


if __name__ == "__main__":
    main()