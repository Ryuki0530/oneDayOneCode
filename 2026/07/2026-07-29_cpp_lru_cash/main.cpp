#include <cstddef>
#include <iostream>
#include <list>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

template <typename Key, typename Value>
class LRUCache {
public:
    LRUCache() = default;

    void put(const Key& key, const Value& value) {
        auto it = cache_.find(key);

        // 既存キーの場合、値を更新して先頭へ移動する
        if (it != cache_.end()) {
            it->second->second = value;

            // splice後もlistのイテレータは有効なまま
            usage_.splice(usage_.begin(), usage_, it->second);
            return;
        }

        // 容量を超える場合、末尾の最も古い要素を削除する
        if (usage_.size() >= CAPACITY) {
            cache_.erase(usage_.back().first);
            usage_.pop_back();
        }

        // 新しい要素を最も最近使われた要素として先頭に追加
        usage_.emplace_front(key, value);
        cache_[key] = usage_.begin();
    }

    std::optional<Value> get(const Key& key) {
        auto it = cache_.find(key);

        if (it == cache_.end()) {
            return std::nullopt;
        }

        // 取得された要素を最も最近使用された位置へ移動
        usage_.splice(usage_.begin(), usage_, it->second);

        return it->second->second;
    }

    void print() const {
        std::cout << "[Most recently used]\n";

        for (const auto& [key, value] : usage_) {
            std::cout << key << " = " << value << '\n';
        }

        std::cout << "[Least recently used]\n";
    }

private:

    static constexpr std::size_t CAPACITY = 3;

    using List = std::list<std::pair<Key, Value>>;
    using ListIt = typename List::iterator;

    std::unordered_map<Key, ListIt> cache_;
    List usage_;
};

int main() {
    LRUCache<std::string, int> cache;

    cache.put("A", 10);
    cache.put("B", 20);
    cache.put("C", 30);

    std::cout << "Initial cache:\n";
    cache.print();

    const auto value = cache.get("A");

    if (value.has_value()) {
        std::cout << "\nget(A): " << value.value() << '\n';
    }

    cache.put("D", 40);

    std::cout << "\nAfter put(D, 40):\n";
    cache.print();

    return 0;
}