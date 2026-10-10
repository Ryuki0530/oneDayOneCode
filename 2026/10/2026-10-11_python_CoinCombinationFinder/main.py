from itertools import product


def find_coins(amount):
	combinations = []

	for coins in product(range(4), repeat=3):
		five_hundred, one_hundred, fifty = coins
		if five_hundred * 500 + one_hundred * 100 + fifty * 50 == amount:
			combinations.append(coins)

	combinations.sort(
		key=lambda coins: (
			sum(coins),
			-coins[0],
			-coins[1],
		)
	)

	if not combinations:
		print("支払い不可")
		return

	for five_hundred, one_hundred, fifty in combinations:
		total_coins = five_hundred + one_hundred + fifty
		print(
			f"500円: {five_hundred}枚, 100円: {one_hundred}枚, "
			f"50円: {fifty}枚 / 合計{total_coins}枚"
		)


def main():
	for amount in (650, 30, 0):
		find_coins(amount)


if __name__ == "__main__":
	main()

