from __future__ import annotations

import argparse
import hashlib
from collections import defaultdict
from pathlib import Path


def iter_files(root: Path):
	yield from (p for p in root.rglob("*") if p.is_file())


def group_by_size(root: Path):
	groups = defaultdict(list)
	for path in iter_files(root):
		try:
			groups[path.stat().st_size].append(path)
		except OSError:
			continue
	return groups


def sha256_of_file(path: Path) -> str:
	digest = hashlib.sha256()
	with path.open("rb") as f:
		for chunk in iter(lambda: f.read(1024 * 1024), b""):
			digest.update(chunk)
	return digest.hexdigest()


def print_duplicates(root: Path) -> None:
	size_groups = group_by_size(root)

	for size in sorted(size_groups):
		candidates = size_groups[size]
		if len(candidates) < 2:
			continue

		hash_groups = defaultdict(list)
		for path in candidates:
			try:
				hash_groups[sha256_of_file(path)].append(path)
			except OSError:
				continue

		duplicate_groups = [paths for paths in hash_groups.values() if len(paths) >= 2]
		if not duplicate_groups:
			continue

		print(f"=== {size} Bytes ===")
		for paths in duplicate_groups:
			for path in sorted(paths):
				print(path.as_posix())
		print()


def parse_args() -> argparse.Namespace:
	parser = argparse.ArgumentParser(description="Duplicate file detector")
	parser.add_argument("path", nargs="?", default=".", help="Directory to scan")
	return parser.parse_args()


def main() -> None:
	args = parse_args()
	root = Path(args.path)

	if not root.exists():
		raise SystemExit(f"Path does not exist: {root}")
	if not root.is_dir():
		raise SystemExit(f"Not a directory: {root}")

	print_duplicates(root)


if __name__ == "__main__":
	main()
