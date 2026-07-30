#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Node {
public:
	explicit Node(std::string name) : name_(std::move(name)) {}

	void addChild(std::unique_ptr<Node> child) {
		children_.push_back(std::move(child));
	}

	void print(int depth = 0) const {
		for (int i = 0; i < depth; ++i) {
			std::cout << "  ";
		}
		std::cout << name_ << '\n';

		for (const auto& child : children_) {
			child->print(depth + 1);
		}
	}

	Node* find(const std::string& name) {
		if (name_ == name) {
			return this;
		}

		for (const auto& child : children_) {
			if (Node* found = child->find(name)) {
				return found;
			}
		}

		return nullptr;
	}

private:
	std::string name_;
	std::vector<std::unique_ptr<Node>> children_;
};

int main() {
	auto root = std::make_unique<Node>("root");

	auto home = std::make_unique<Node>("home");
	home->addChild(std::make_unique<Node>("user"));
	home->addChild(std::make_unique<Node>("guest"));

	root->addChild(std::move(home));
	root->addChild(std::make_unique<Node>("etc"));

	root->print();

	Node* result = root->find("guest");
	if (result != nullptr) {
		std::cout << "Found" << std::endl;
	} else {
		std::cout << "Not found" << std::endl;
	}

	return 0;
}
