def apply_transactions(stock, transactions):
	updated = stock.copy()
	failed = []

	for index, (product, change) in enumerate(transactions):
		new_quantity = updated.get(product, 0) + change
		if new_quantity < 0:
			failed.append(index)
			continue
		updated[product] = new_quantity

	return updated, failed


if __name__ == "__main__":
	stock = {"apple": 5, "banana": 2}
	transactions = [
		("apple", -3),
		("banana", -4),
		("orange", 3),
		("apple", -3),
		("banana", 1),
		("grape", -1),
	]

	updated, failed = apply_transactions(stock, transactions)
	print(updated)
	print(failed)
	print(stock)


