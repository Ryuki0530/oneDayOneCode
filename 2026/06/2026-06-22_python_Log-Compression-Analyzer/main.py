def compress(s):
    if not s:
        return ""
    
    result = []
    current_char = s[0]
    count = 1
    
    for i in range(1, len(s)):
        if s[i] == current_char:
            count += 1
        else:
            result.append(f"{current_char}{count}")
            current_char = s[i]
            count = 1
    
    result.append(f"{current_char}{count}")
    return "".join(result)


def decompress(s):
    result = []
    i = 0
    
    while i < len(s):
        char = s[i]
        count_str = ""
        i += 1
        
        while i < len(s) and s[i].isdigit():
            count_str += s[i]
            i += 1
        
        count = int(count_str)
        result.append(char * count)
    
    return "".join(result)


if __name__ == "__main__":
    input_string = input()
    compressed = compress(input_string)
    print(compressed)
