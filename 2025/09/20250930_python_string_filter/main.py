import sys

def is_alnum_only(s):
    return s.isalnum()

def main():
    lines = [line.rstrip('\n') for line in sys.stdin]
    filtered = [s for s in lines if is_alnum_only(s)]
    filtered.sort(key=lambda x: (len(x), x))
    for s in filtered:
        print(s)

if __name__ == "__main__":
    main()