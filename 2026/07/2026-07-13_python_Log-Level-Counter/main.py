logs = [
	"INFO: Start",
	"ERROR: File not found",
	"INFO: Load config",
	"WARNING: Low memory",
	"ERROR: Connection failed",
	"INFO: Ready",
]


def count_log_levels(logs):
	counts = {}
	for log in logs:
		level = log.split(":", 1)[0]
		if level not in counts:
			counts[level] = 0
		counts[level] += 1
	return counts


counts = count_log_levels(logs)

for level, count in sorted(counts.items(), key=lambda item: (-item[1], item[0])):
	print(f"{level}: {count}")
