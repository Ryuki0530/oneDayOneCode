
#!/usr/bin/env python3
"""
Human Terminal Recorder

Simple CLI recorder that logs commands, outputs and timings to history.json.

Features:
- record mode (default)
- history show
- history grep <term>
- history replay <index>
- history top

Only Python standard library is used.
"""
import sys
import json
import shlex
import subprocess
from pathlib import Path
from datetime import datetime
from time import perf_counter

HISTORY_FILE = Path.cwd() / "history.json"


def load_history():
	if HISTORY_FILE.exists():
		try:
			return json.loads(HISTORY_FILE.read_text())
		except Exception:
			return []
	return []


def save_history(h):
	HISTORY_FILE.write_text(json.dumps(h, ensure_ascii=False, indent=2))


def format_entry(i, e):
	t = e.get("time")
	cmd = e.get("command")
	rc = e.get("returncode")
	return f"{i:3d}: [{t}] rc={rc} cmd={cmd}"


def cmd_history_show(args):
	h = load_history()
	if len(args) >= 2 and args[1] == "grep":
		term = args[2] if len(args) > 2 else ""
		for i, e in enumerate(h, 1):
			if term in e.get("command", ""):
				print(format_entry(i, e))
		return
	if len(args) >= 2 and args[1] == "replay":
		if len(args) < 3:
			print("replay requires an index")
			return
		try:
			idx = int(args[2]) - 1
		except ValueError:
			print("invalid index")
			return
		if idx < 0 or idx >= len(h):
			print("index out of range")
			return
		entry = h[idx]
		confirm = input(f"Replay #{idx+1}: {entry.get('command')} ? (y/N) ")
		if confirm.lower() != "y":
			print("aborted")
			return
		run_and_record(entry.get("command"))
		return
	if len(args) >= 2 and args[1] == "top":
		from collections import Counter

		h = load_history()
		cnt = Counter()
		for e in h:
			# take first token as command name
			tok = shlex.split(e.get("command", ""))
			if tok:
				cnt[tok[0]] += 1
		print("Top Commands")
		for name, c in cnt.most_common(10):
			print(f"{name:10s} {c}")
		return

	# default show
	for i, e in enumerate(h, 1):
		print(format_entry(i, e))


def run_and_record(command):
	# record timings
	start_dt = datetime.utcnow().isoformat()
	t0 = perf_counter()
	try:
		# run in shell so complex commands work
		proc = subprocess.run(command, shell=True, capture_output=True, text=True)
	except Exception as exc:
		proc = None
		err = str(exc)
	t1 = perf_counter()
	end_dt = datetime.utcnow().isoformat()
	duration_ms = int((t1 - t0) * 1000)

	entry = {
		"time": start_dt,
		"command": command,
		"returncode": proc.returncode if proc is not None else 1,
		"stdout": proc.stdout if proc is not None else "",
		"stderr": proc.stderr if proc is not None else (err if proc is None else ""),
		"start": start_dt,
		"end": end_dt,
		"duration_ms": duration_ms,
	}

	h = load_history()
	h.append(entry)
	save_history(h)

	# print outputs to user
	if entry["stdout"]:
		print(entry["stdout"], end="")
	if entry["stderr"]:
		print(entry["stderr"], end="", file=sys.stderr)
	return entry


def repl():
	print("record")
	while True:
		try:
			line = input("$ ")
		except EOFError:
			print()
			break
		line = line.strip()
		if not line:
			continue
		if line == "exit":
			break
		if line.startswith("history"):
			parts = shlex.split(line)
			cmd_history_show(parts)
			continue
		run_and_record(line)


def main():
	repl()


if __name__ == "__main__":
	main()

