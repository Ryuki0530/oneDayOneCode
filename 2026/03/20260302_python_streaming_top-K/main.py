import sys
import heapq
from collections import defaultdict

def main():
    data = sys.stdin.buffer.read().splitlines()
    if not data:
        return

    Q, K = map(int, data[0].split())
    raw_scores = defaultdict(int)
    global_offset = 0
    version = defaultdict(int)
    heap = []
    out = []

    for i in range(1, min(len(data), Q + 1)):
        parts = data[i].split()
        cmd = parts[0]

        if cmd == b"ADD":
            name = parts[1]
            x = int(parts[2])
            current = raw_scores[name] - global_offset
            if current < 0:
                current = 0
            new_score = current + x
            raw_scores[name] = new_score + global_offset
            version[name] += 1
            ver = version[name]
            heapq.heappush(heap, (-new_score, name, ver))

        elif cmd == b"DECAY":
            d = int(parts[1])
            global_offset += d

        else:
            picked = []
            used_names = set()

            while len(picked) < K and heap:
                neg_score, name, ver = heapq.heappop(heap)

                if ver != version[name]:
                    continue

                actual = raw_scores[name] - global_offset
                if actual <= 0:
                    continue

                if name in used_names:
                    continue

                used_names.add(name)
                picked.append((actual, name, ver))

            if not picked:
                out.append("EMPTY")
            else:
                picked.sort(key=lambda t: (-t[0], t[1]))
                for score, name, ver in picked:
                    out.append(f"{name.decode()} {score}")

                for score, name, ver in picked:
                    heapq.heappush(heap, (-score, name, ver))

    sys.stdout.write("\n".join(out) + ("\n" if out else ""))

if __name__ == "__main__":
    main()