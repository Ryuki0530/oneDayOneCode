numbers = list(map(int, input().split()))
even_numbers = [n for n in numbers if n % 2 == 0]
even_numbers.sort()
print(*even_numbers)