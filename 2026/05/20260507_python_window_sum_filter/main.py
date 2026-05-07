import sys


def main() -> None:

    #EOF文字除去
    raw = sys.stdin.buffer.read().replace(b"\x1a", b"")

    data = list(map(int, raw.split()))
    if not data:
        return

    n, k, t = data[0], data[1], data[2]
    a = data[3:3 + n]

    window_sum = sum(a[:k])
    count = 1 if window_sum >= t else 0

    for i in range(k, n):
        window_sum += a[i] - a[i - k]
        if window_sum >= t:
            count += 1

    print(count)


if __name__ == "__main__":
    main()