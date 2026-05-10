import sys
import heapq


def main():
	input = sys.stdin.readline
	q = int(input())
	heap = []
	order = 0
	out = []

	for _ in range(q):
		parts = input().split()
		if parts[0] == 'ADD':
			name = parts[1]
			priority = int(parts[2])
			heapq.heappush(heap, (priority, order, name))
			order += 1
		else:
			if heap:
				_, _, name = heapq.heappop(heap)
				out.append(name)
			else:
				out.append('NONE')

	sys.stdout.write('\n'.join(out))


if __name__ == '__main__':
	main()
