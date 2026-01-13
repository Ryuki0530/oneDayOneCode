def main():
    N, Q = map(int, input().split())
    
    diff = [0] * (N + 1)
    
    for _ in range(Q):
        l, r, x = map(int, input().split())
        diff[l - 1] += x
        diff[r] -= x
    
    result = []
    current = 0
    for i in range(N):
        current += diff[i]
        result.append(current)
    
    print(' '.join(map(str, result)))

if __name__ == "__main__":
    main()