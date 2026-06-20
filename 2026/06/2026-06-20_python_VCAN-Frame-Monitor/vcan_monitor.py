from __future__ import annotations

import signal
import sys
import time
from dataclasses import dataclass
from typing import Dict, Optional
import can

@dataclass
class FrameStats:
	count: int = 0
	last_data: str = ""
	last_rx_time: Optional[float] = None
	previous_rx_time: Optional[float] = None
	total_period: float = 0.0

	def update(self, rx_time: float, data: str) -> None:
		if self.last_rx_time is not None:
			self.total_period += rx_time - self.last_rx_time
		self.previous_rx_time = self.last_rx_time
		self.last_rx_time = rx_time
		self.last_data = data
		self.count += 1

	@property
	def elapsed_since_previous(self) -> str:
		if self.previous_rx_time is None or self.last_rx_time is None:
			return "N/A"
		return f"{self.last_rx_time - self.previous_rx_time:.6f}s"

	@property
	def average_period(self) -> str:
		if self.count < 2:
			return "N/A"
		return f"{self.total_period / (self.count - 1):.6f}s"


def format_can_id(can_id: int) -> str:
	return f"0x{can_id:03X}" if can_id <= 0x7FF else f"0x{can_id:08X}"


def format_data(data: bytes) -> str:
	return data.hex().upper() if data else ""


def print_summary(stats_by_id: Dict[int, FrameStats]) -> None:
	print("\n--- CAN Frame Summary ---")
	print(f"{'CAN ID':>10}  {'Count':>7}  {'Latest Data':>18}  {'Elapsed':>12}  {'Avg Period':>12}")
	for can_id in sorted(stats_by_id):
		stats = stats_by_id[can_id]
		print(
			f"{format_can_id(can_id):>10}  {stats.count:7d}  {stats.last_data:>18}  "
			f"{stats.elapsed_since_previous:>12}  {stats.average_period:>12}"
		)


def main() -> int:
	stop_requested = False

	def handle_sigint(signum, frame):
		nonlocal stop_requested
		stop_requested = True

	signal.signal(signal.SIGINT, handle_sigint)
	bus = can.interface.Bus(channel="vcan0", bustype="socketcan")
	stats_by_id: Dict[int, FrameStats] = {}

	try:
		while not stop_requested:
			msg = bus.recv(timeout=0.5)
			if msg is None:
				continue

			rx_time = time.time()
			can_id = msg.arbitration_id
			data = format_data(msg.data)

			stats = stats_by_id.setdefault(can_id, FrameStats())
			stats.update(rx_time, data)

			print(
				f"CAN ID={format_can_id(can_id)} count={stats.count} "
				f"latest={stats.last_data} elapsed={stats.elapsed_since_previous} "
				f"avg={stats.average_period}"
			)
	finally:
		try:
			bus.shutdown()
		except Exception:
			pass
		print_summary(stats_by_id)

	return 0


if __name__ == "__main__":
	raise SystemExit(main())
