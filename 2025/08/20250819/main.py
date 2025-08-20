def main():
    numbers = list(map(int,input().split()))
    even_count = 0
    odd_count = 0
    for number in numbers:
        if number % 2 == 0:
            even_count += 1
        else:
            odd_count += 1

    print(f"偶数: {even_count} 個")
    print(f"奇数: {odd_count} 個")

if __name__ == "__main__":
    main()