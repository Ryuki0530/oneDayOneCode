import sys
from collections import defaultdict

def main():
    input = sys.stdin.readline
    R, B = map(float, input().split())
    N = int(input())
    
    ip_data = defaultdict(lambda: {'last_t': 0, 'tokens': B})
    
    results = []
    
    for _ in range(N):
        t, ip, path = input().strip().split()
        t = int(t)
        

        if ip_data[ip]['last_t'] != 0:
            elapsed_time = t - ip_data[ip]['last_t']
            ip_data[ip]['tokens'] = min(B, ip_data[ip]['tokens'] + elapsed_time * R)
        
        if ip_data[ip]['tokens'] >= 1:
            results.append(f"{t} {ip} {path} ALLOW")
            ip_data[ip]['tokens'] -= 1 
        else:
            results.append(f"{t} {ip} {path} BLOCK")
        
        ip_data[ip]['last_t'] = t
    
    sys.stdout.write("\n".join(results) + "\n")

if __name__ == "__main__":
    main()