from __future__ import annotations

import argparse
import hashlib
import time
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path


def iter_files(root: Path):
	for path in root.rglob("*"):
		if path.is_file():
			yield path


def sha256_file(path: Path, chunk_size: int = 1024 * 1024) -> str:
	digest = hashlib.sha256()
	with path.open("rb") as f:
		while True:
			chunk = f.read(chunk_size)
			if not chunk:
				break
			digest.update(chunk)
	return digest.hexdigest()


def scan(root: Path, max_workers: int):
	files = list(iter_files(root))
	results = []
	with ThreadPoolExecutor(max_workers=max_workers) as executor:
		for path, sha256 in zip(files, executor.map(sha256_file, files)):
			results.append((path, path.stat().st_size, sha256))
	return results


def print_results(results):
	groups = defaultdict(list)
	for path, size, sha256 in results:
		print(path.name)
		print(f"  Size : {size} bytes")
		print(f"  SHA256 : {sha256}")
		print()
		groups[sha256].append(path)

	print(f"Processed {len(results)} files.")

	duplicates = {sha256: paths for sha256, paths in groups.items() if len(paths) > 1}
	if duplicates:
		print()
		print("Duplicate files:")
		for sha256, paths in duplicates.items():
			print(sha256)
			for path in paths:
				print(f"  {path}")


def benchmark(root: Path):
	for workers in (1, 4, 8):
		start = time.perf_counter()
		results = scan(root, max_workers=workers)
		elapsed = time.perf_counter() - start
		print(f"max_workers={workers}: {elapsed:.3f}s ({len(results)} files)")


def main() -> None:
	parser = argparse.ArgumentParser(description="Concurrent file hash scanner")
	parser.add_argument("directory", nargs="?", default=".", help="Target directory")
	parser.add_argument("--workers", type=int, default=4, help="Number of worker threads")
	parser.add_argument("--benchmark", action="store_true", help="Compare execution times")
	args = parser.parse_args()

	root = Path(args.directory)
	if not root.exists() or not root.is_dir():
		raise SystemExit(f"Invalid directory: {root}")

	if args.benchmark:
		benchmark(root)
		return

	results = scan(root, max_workers=args.workers)
	print_results(results)


if __name__ == "__main__":
	main()
