#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Node {
public:
    explicit Node(std::string name);

    void addChild(std::unique_ptr<Node> child);

    void print(int depth = 0) const;

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

    return 0;
}