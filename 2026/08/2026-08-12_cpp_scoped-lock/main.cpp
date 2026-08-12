#include <iostream>
#include <mutex>
#include <string>
#include <thread>

class BankAccount {
public:
	BankAccount(std::string name, int balance)
		: name_(std::move(name)), balance_(balance) {}

	const std::string& name() const {
		return name_;
	}

	int balance() const {
		std::scoped_lock lock(mutex_);
		return balance_;
	}

private:
	std::string name_;
	int balance_;
	mutable std::mutex mutex_;

	friend bool transfer(BankAccount& from, BankAccount& to, int amount);
};

bool transfer(BankAccount& from, BankAccount& to, int amount) {
	std::scoped_lock lock(from.mutex_, to.mutex_);

	if (from.balance_ < amount) {
		return false;
	}

	from.balance_ -= amount;
	to.balance_ += amount;
	return true;
}

int main() {
	BankAccount alice("Alice", 10000);
	BankAccount bob("Bob", 10000);

	std::thread t1([&]() {
		for (int i = 0; i < 5000; ++i) {
			transfer(alice, bob, 1);
		}
	});

	std::thread t2([&]() {
		for (int i = 0; i < 5000; ++i) {
			transfer(bob, alice, 1);
		}
	});

	t1.join();
	t2.join();

	const int alice_balance = alice.balance();
	const int bob_balance = bob.balance();

	std::cout << alice.name() << ": " << alice_balance << '\n';
	std::cout << bob.name() << ": " << bob_balance << '\n';
	std::cout << "Total: " << (alice_balance + bob_balance) << '\n';

	return 0;
}
