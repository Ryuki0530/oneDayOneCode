import sys
from collections import defaultdict

def main():
    user_counts = defaultdict(int)
    page_counts = defaultdict(int)
    total = 0

    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        user_id, page = line.split()
        user_counts[user_id] += 1
        page_counts[page] += 1
        total += 1

    print(f"TOTAL {total}")

    if total == 0:
        print("TOP_USER NONE 0")
        print("TOP_PAGE NONE 0")
    else:
        top_user = sorted(user_counts.items(), key=lambda x: (-x[1], x[0]))[0]
        top_page = sorted(page_counts.items(), key=lambda x: (-x[1], x[0]))[0]
        print(f"TOP_USER {top_user[0]} {top_user[1]}")
        print(f"TOP_PAGE {top_page[0]} {top_page[1]}")

    print("\nUSERS")
    for user, count in sorted(user_counts.items(), key=lambda x: (-x[1], x[0])):
        print(f"{user} {count}")

    print("\nPAGES")
    for page, count in sorted(page_counts.items(), key=lambda x: (-x[1], x[0])):
        print(f"{page} {count}")

if __name__ == "__main__":
    main()