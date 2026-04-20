from collections import defaultdict
from pathlib import Path
import sys


def summarize_log(log_path: Path) -> dict[str, dict[str, int]]:
    summary: dict[str, dict[str, int]] = defaultdict(lambda: {"total": 0, "success": 0})

    with log_path.open("r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue

            try:
                user, status = line.split(",", 1)
            except ValueError:
                continue

            user = user.strip()
            status = status.strip()

            summary[user]["total"] += 1
            if status == "200":
                summary[user]["success"] += 1

    return dict(summary)


def main() -> int:
    log_path = Path("log.txt")

    try:
        summary = summarize_log(log_path)
    except FileNotFoundError:
        print("Error: log.txt not found")
        return 1

    for user in sorted(summary):
        data = summary[user]
        print(f"{user}: total={data['total']} success={data['success']}")

    return 0


if __name__ == "__main__":
    sys.exit(main())