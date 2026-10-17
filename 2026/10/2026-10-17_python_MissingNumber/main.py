def find_missing(numbers, n):
	return n * (n + 1) // 2 - sum(numbers)


print(find_missing([2], 2))
print(find_missing([3, 1, 2], 4))