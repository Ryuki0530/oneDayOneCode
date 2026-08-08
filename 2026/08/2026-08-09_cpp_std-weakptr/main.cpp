#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Node : public std::enable_shared_from_this<Node> {
public:
    explicit Node(std::string name) : name_(name) {}

    void addChild(const std::shared_ptr<Node>& child) {
        children_.push_back(child);
        child->parent_ = shared_from_this();
    }

    void printParent() const {
        if (auto parent = parent_.lock()) {
            std::cout << "Parent of child: " << parent->name_ << '\n';
        } else {
            std::cout << "Parent expired\n";
        }
    }

    ~Node() {
        std::cout << "Destroyed: " << name_ << '\n';
    }

private:
    std::string name_;
    std::vector<std::shared_ptr<Node>> children_;
    std::weak_ptr<Node> parent_;
};

int main() {
    auto root = std::make_shared<Node>("root");
    auto child = std::make_shared<Node>("child");

    root->addChild(child);
    child->printParent();

    return 0;
}
