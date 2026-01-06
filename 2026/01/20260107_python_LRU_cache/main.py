from collections import OrderedDict

class LRUCache:
    def __init__(self, capacity):
        self.capacity = capacity
        self.cache = OrderedDict()
    
    def get(self, key):
        if key not in self.cache:
            return -1
        
        self.cache.move_to_end(key)
        return self.cache[key]
    
    def put(self, key, value):
        if key in self.cache:
            self.cache.move_to_end(key)
            self.cache[key] = value
        else:
            self.cache[key] = value
            if len(self.cache) > self.capacity:
                self.cache.popitem(last=False)

C, Q = map(int, input().split())
lru = LRUCache(C)

for _ in range(Q):
    operation = input().split()
    if operation[0] == "get":
        key = int(operation[1])
        print(lru.get(key))
    else:
        key = int(operation[1])
        value = int(operation[2])
        lru.put(key, value)