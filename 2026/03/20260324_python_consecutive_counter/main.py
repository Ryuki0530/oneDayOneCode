def main():
    numbers = list(map(int, input().split()))
    current_value = numbers[0]
    count = 1    
    result = []
    
    for i in range(1, len(numbers)):
        if numbers[i] == current_value:
            count += 1
        else:
            result.append(f"{current_value} {count}")
            current_value = numbers[i]
            count = 1
    
    result.append(f"{current_value} {count}")
    print("\n".join(result))

if __name__ == "__main__":
    main()