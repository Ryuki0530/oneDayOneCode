import sys

def main():
    data = sys.stdin.read().strip().split()
    if not data:
        return
    it = iter(data)
    try:
        n = int(next(it))
    except StopIteration:
        return
    times = []
    for _ in range(n):
        try:
            times.append(int(next(it)))
        except StopIteration:
            break

    for t in times:
        if t < 30:
            print("OK")
        elif t < 60:
            print("REST")
        else:
            print("STOP")

if __name__ == '__main__':
    main()
