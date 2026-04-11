# main.py

def main():
    n = int(input())
    totals = {}

    for _ in range(n):
        name, value = input().split()
        value = int(value)

        if name in totals:
            totals[name] += value
        else:
            totals[name] = value

    result = sorted(totals.items(), key=lambda x: (-x[1], x[0]))

    for name, total in result:
        print(name, total)


if __name__ == "__main__":
    main()