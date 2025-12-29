import sys

def minimum_rotation(s):
    s = s.strip()
    n = len(s)
    if n == 0:
        return 0, ""
    
    doubled = s * 2
    failure = [-1] * (2 * n)
    min_start = 0

    for j in range(1, 2 * n):
        i = failure[j - min_start - 1]
        while i != -1 and doubled[j] != doubled[min_start + i + 1]:
            if doubled[j] < doubled[min_start + i + 1]:
                min_start = j - i - 1
            i = failure[i]
        if i == -1 and doubled[j] != doubled[min_start]:
            if doubled[j] < doubled[min_start]:
                min_start = j
            failure[j - min_start] = -1
        else:
            failure[j - min_start] = i + 1
    pos = min_start % n
    return pos, doubled[pos:pos + n]

if __name__ == "__main__":
    input_str = sys.stdin.readline().rstrip('\n')
    pos, min_rot = minimum_rotation(input_str)
    print(f"最小回転の開始位置: {pos}")
    print(f"最小回転文字列: {min_rot}")
    print(f"元の文字列を{pos}文字左に回転すると最小になります。")
    print(f"例: {input_str[pos:] + input_str[:pos]}")
