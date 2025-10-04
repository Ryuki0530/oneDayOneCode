numbers = list(map(int, input().split()))
even_sum = sum(n for n in numbers if n % 2 == 0)
print(even_sum)