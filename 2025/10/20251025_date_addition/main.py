import sys
from dataclasses import dataclass
from datetime import date, timedelta
from typing import TextIO

@dataclass(frozen=True)
class DateFormatter:
    def format(self, d: date) -> str:
        return d.isoformat()


@dataclass(frozen=True)
class DateParser:
    def parse(self, s: str) -> date:
        # Expects YYYY-MM-DD
        return date.fromisoformat(s.strip())


@dataclass(frozen=True)
class DateCalculator:
    def add_days(self, d: date, days: int) -> date:
        return d + timedelta(days=days)


class DateAdditionApp:
    def __init__(
        self,
        parser: DateParser | None = None,
        formatter: DateFormatter | None = None,
        calculator: DateCalculator | None = None,
    ) -> None:
        self.parser = parser or DateParser()
        self.formatter = formatter or DateFormatter()
        self.calculator = calculator or DateCalculator()

    def run(self, in_stream: TextIO = sys.stdin, out_stream: TextIO = sys.stdout) -> None:
        base_str = in_stream.readline().strip()
        n_str = in_stream.readline().strip()

        base_date = self.parser.parse(base_str)
        n_days = int(n_str)

        result = self.calculator.add_days(base_date, n_days)
        out_stream.write(self.formatter.format(result) + "\n")


if __name__ == "__main__":
    DateAdditionApp().run()