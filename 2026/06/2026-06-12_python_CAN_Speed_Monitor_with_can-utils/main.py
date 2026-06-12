import argparse
import subprocess
import sys

SPEED_CAN_ID = "100"
CAN_INTERFACE = "vcan0"
SPEED_THRESHOLD = 60

def monitor_can(interface: str, threshold: int) -> None:
	proc = subprocess.Popen(
		["candump", interface],
		stdout=subprocess.PIPE,
		stderr=subprocess.DEVNULL,
		text=True,
	)

	try:
		assert proc.stdout is not None
		for line in proc.stdout:
			parts = line.split()
			if len(parts) < 4:
				continue

			can_id = parts[1]
			if can_id.lower() != SPEED_CAN_ID:
				continue

			try:
				speed = int(parts[3], 16)
			except ValueError:
				continue

			status = "WARN" if speed >= threshold else "OK"
			print(f"speed={speed} km/h {status}")
			sys.stdout.flush()
	except KeyboardInterrupt:
		pass
	finally:
		proc.terminate()
		try:
			proc.wait(timeout=1)
		except subprocess.TimeoutExpired:
			proc.kill()


def main() -> None:
	parser = argparse.ArgumentParser(description="CAN speed monitor with candump")
	parser.add_argument("--interface", default=CAN_INTERFACE, help="CAN interface name")
	parser.add_argument("--threshold", type=int, default=SPEED_THRESHOLD, help="warning threshold")
	parser.add_argument("--CAN_ID", default=SPEED_CAN_ID, help="CAN ID to monitor (hex)")
	
	args = parser.parse_args()

	monitor_can(args.interface, args.threshold)


if __name__ == "__main__":
	main()
