from __future__ import annotations

import json
import os
import sys
from pathlib import Path


snapshot_old = {
	"main.py": 1200,
	"config.json": 512,
	"README.md": 850,
	"data.csv": 15000,
}

snapshot_new = {
	"main.py": 1400,
	"README.md": 850,
	"image.png": 3000,
	"data.csv": 15000,
}


def build_snapshot(root: str | Path) -> dict[str, int]:
	root_path = Path(root)
	snapshot: dict[str, int] = {}

	for dirpath, _, filenames in os.walk(root_path):
		current_dir = Path(dirpath)
		for filename in filenames:
			file_path = current_dir / filename
			rel_path = file_path.relative_to(root_path).as_posix()
			snapshot[rel_path] = file_path.stat().st_size

	return snapshot


def diff_snapshots(old: dict[str, int], new: dict[str, int]) -> dict[str, list[str]]:
	old_keys = set(old)
	new_keys = set(new)

	added = sorted(new_keys - old_keys)
	removed = sorted(old_keys - new_keys)
	common = sorted(old_keys & new_keys)

	modified = [name for name in common if old[name] != new[name]]
	unchanged = [name for name in common if old[name] == new[name]]

	return {
		"added": added,
		"removed": removed,
		"modified": modified,
		"unchanged": unchanged,
	}


def print_diff(diff: dict[str, list[str]], old: dict[str, int] | None = None, new: dict[str, int] | None = None) -> None:
	print("Added:")
	print("\n".join(diff["added"]))
	print()

	print("Removed:")
	print("\n".join(diff["removed"]))
	print()

	print("Modified:")
	if old is not None and new is not None:
		print("\n".join(f"{name} ({old[name]} -> {new[name]})" for name in diff["modified"]))
	else:
		print("\n".join(diff["modified"]))
	print()

	print("Unchanged:")
	print("\n".join(diff["unchanged"]))


def save_snapshot(directory: str, output_json: str) -> None:
	snapshot = build_snapshot(directory)
	Path(output_json).write_text(json.dumps(snapshot, ensure_ascii=False, indent=2), encoding="utf-8")


def load_snapshot(json_path: str) -> dict[str, int]:
	return json.loads(Path(json_path).read_text(encoding="utf-8"))


def main(argv: list[str]) -> int:
	if len(argv) == 1:
		diff = diff_snapshots(snapshot_old, snapshot_new)
		print_diff(diff, snapshot_old, snapshot_new)
		return 0

	if len(argv) == 4 and argv[1] == "save":
		save_snapshot(argv[2], argv[3])
		return 0

	if len(argv) == 4 and argv[1] == "diff":
		old_snapshot = load_snapshot(argv[2])
		new_snapshot = load_snapshot(argv[3])
		diff = diff_snapshots(old_snapshot, new_snapshot)
		print_diff(diff, old_snapshot, new_snapshot)
		return 0

	print("Usage:")
	print("  python snapshot.py")
	print("  python snapshot.py save <directory> <output.json>")
	print("  python snapshot.py diff <old.json> <new.json>")
	return 1


if __name__ == "__main__":
	raise SystemExit(main(sys.argv))
