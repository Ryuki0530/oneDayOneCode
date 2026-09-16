from collections import defaultdict


def summarize_sales(sales):
	summary = defaultdict(lambda: {"total": 0, "count": 0})

	for product, amount in sales:
		summary[product]["total"] += amount
		summary[product]["count"] += 1

	return {
		product: {
			"total": values["total"],
			"count": values["count"],
			"average": values["total"] / values["count"],
		}
		for product, values in summary.items()
	}


if __name__ == "__main__":
	sales = [
		("apple", 120),
		("banana", 80),
		("apple", 150),
		("orange", 100),
		("banana", 70),
		("apple", 130),
	]

	summary = summarize_sales(sales)
	for product, values in summary.items():
		print(
			f"{product}: total={values['total']}, "
			f"count={values['count']}, average={values['average']:.2f}"
		)

	best_seller = max(summary, key=lambda product: summary[product]["total"])
	print(f"Best seller: {best_seller}")
