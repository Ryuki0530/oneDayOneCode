even = 0
odd = 0
nums = list(map(int, input().split()))
for num in nums:
    if num%2 == 0 :
        even += 1
    else:
        odd += 1
print("偶数:", even,"個")
print("奇数:", odd,"個")