import heapq
import sys

def streaming_top_k():
    n, k = map(int, input().split())
    
    heap = []
    
    for _ in range(n):
        num = int(input())
        
        if len(heap) < k:
            heapq.heappush(heap, num)
        elif num > heap[0]:
            heapq.heapreplace(heap, num)
    
    result = sorted(heap, reverse=True)
    
    for val in result:
        print(val)

if __name__ == "__main__":
    streaming_top_k()