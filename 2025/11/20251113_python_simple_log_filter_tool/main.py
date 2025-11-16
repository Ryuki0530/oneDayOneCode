import sys
from pathlib import Path

def main() -> None:
    if len(sys.argv) != 3:
        script = Path(sys.argv[0]).name
        print(f"Usage: python {script} <logfile> <keyword>")
        return

    log_path = Path(sys.argv[1])
    keyword = sys.argv[2]

    if not log_path.exists():
        print(f"File not found: {log_path}")
        return

    try:
        with log_path.open("r", encoding="utf-8") as log_file:
            for line in log_file:
                if keyword in line:
                    print(line, end="")
    except OSError as exc:
        print(f"Error reading file: {exc}")


if __name__ == "__main__":
    main()