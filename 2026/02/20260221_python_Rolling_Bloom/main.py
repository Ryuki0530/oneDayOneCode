import sys
import hashlib

def get_hash_indices(x, k, m, seed_base=0):
    indices = []
    for i in range(k):
        h = hashlib.blake2b(str(x).encode(), digest_size=8, salt=str(seed_base + i).encode()[:16].ljust(16, b'\0'))
        idx = int.from_bytes(h.digest(), 'big') % m
        indices.append(idx)
    return indices

def set_bit(bits, idx):
    bits[idx >> 3] |= 1 << (idx & 7)

def get_bit(bits, idx):
    return (bits[idx >> 3] >> (idx & 7)) & 1

def check_or_add(current, previous, indices, m):
    found = any(get_bit(current, idx) or get_bit(previous, idx) for idx in indices)
    for idx in indices:
        set_bit(current, idx)
    return 1 if found else 0

def main():
    line = sys.stdin.readline().strip()
    w, k, m = map(int, line.split())
    
    byte_size = (m + 7) // 8
    current = bytearray(byte_size)
    previous = bytearray(byte_size)
    
    count = 0
    
    for line in sys.stdin:
        x = int(line.strip())
        
        indices = get_hash_indices(x, k, m)
        
        result = check_or_add(current, previous, indices, m)
        print(result)
        
        count += 1
        
        if count % w == 0:
            previous = current
            current = bytearray(byte_size)

if __name__ == "__main__":
    main()