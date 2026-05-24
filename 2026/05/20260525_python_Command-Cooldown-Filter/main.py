
import sys


def main():
	data = sys.stdin.buffer.read().split()
	if not data:
		return
	it = iter(data)
	N = int(next(it))
	K = int(next(it))
	last = {}
	accepted = []
	for _ in range(N):
		t = int(next(it))
		cmd = next(it).decode()
		prev = last.get(cmd)
		if prev is None or t - prev >= K:
			accepted.append((t, cmd))
			last[cmd] = t

	out_lines = [str(len(accepted))]
	out_lines += [f"{t} {cmd}" for t, cmd in accepted]
	sys.stdout.write("\n".join(out_lines))


if __name__ == "__main__":
	main()
