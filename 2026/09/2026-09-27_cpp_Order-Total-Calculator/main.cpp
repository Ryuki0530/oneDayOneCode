#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Item {
	std::string name;
	int price;
	int quantity;
};

int main() {
	const std::vector<Item> items{
		{"Apple", 150, 3},
		{"Coffee", 500, 2},
		{"Bread", 200, 4},
		{"Milk", 250, 1}
	};

	const int total = std::accumulate(
		items.begin(), items.end(), 0,
		[](int sum, const Item& item) {
			return sum + item.price * item.quantity;
		});

	std::cout << "Total: " << total << " yen\n";
	return 0;
}
