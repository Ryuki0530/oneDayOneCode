from __future__ import annotations
import argparse
import sys
from typing import Optional
import pyjokes

def get_joke_avoid_repeat(last: Optional[str]) -> str:
        """Get a joke, avoiding immediate repeats when possible."""
        for _ in range(10):
                joke = pyjokes.get_joke()
                if joke != last:
                        return joke
        return joke  # fallback if always the same


def interactive_mode() -> int:
        print("=== Joke Roulette ===\n")
        print("Press Enter to get a joke.")
        print("Type q to quit.\n")
        count = 0
        last_joke: Optional[str] = None

        while True:
                try:
                        user = input("> ")
                except (EOFError, KeyboardInterrupt):
                        print()  # newline
                        break

                if user is None:
                        user = ""
                if user.strip().lower() == "q":
                        break

                # treat empty or whitespace-only as request for a joke
                joke = get_joke_avoid_repeat(last_joke)
                print(joke + "\n")
                last_joke = joke
                count += 1

        return count


def count_mode(total: int) -> int:
        last_joke: Optional[str] = None
        shown = 0
        for _ in range(total):
                joke = get_joke_avoid_repeat(last_joke)
                print(joke + ("\n" if _ < total - 1 else ""))
                last_joke = joke
                shown += 1
        return shown


def main() -> None:
        parser = argparse.ArgumentParser(description="Joke Roulette CLI (pyjokes)")
        parser.add_argument("-c", "--count", type=int, default=0, help="Show COUNT jokes then exit")
        args = parser.parse_args()

        if args.count and args.count > 0:
                shown = count_mode(args.count)
        else:
                shown = interactive_mode()

        print("Goodbye!")
        # extra info: number of jokes shown (optional)
        if shown:
                print(f"Displayed {shown} joke{'s' if shown != 1 else ''}.")


if __name__ == "__main__":
        main()