#include <array>
#include <cstddef>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

template <typename T, std::size_t Capacity>
class RingBuffer {
public:
    void push(T value){
        const std::size_t write_index = (head_ + size_) % Capacity;
        data_[write_index] = std::move(value);

        if (full()) {
            head_ = (head_ + 1) % Capacity;
        } else {
            ++size_;
        }
    };
    std::optional<T> pop(){
        if (empty()) {
            return std::nullopt;
        }

        std::optional<T> value = std::move(data_[head_]);
        data_[head_] = std::nullopt;
        head_ = (head_ + 1) % Capacity;
        --size_;
        return value;
    };

    const T& front() const{
        if (empty()) {
            throw std::out_of_range("RingBuffer is empty");
        }
        return data_[head_].value();
    };
    const T& back() const{
        if (empty()) {
            throw std::out_of_range("RingBuffer is empty");
        }
        const std::size_t back_index = (head_ + size_ - 1) % Capacity;
        return data_[back_index].value();
    };

    bool empty() const{
        return size_ == 0;
    };
    bool full() const{
        return size_ == Capacity;
    };
    std::size_t size() const{
        return size_;
    };

private:
    static_assert(Capacity > 0, "RingBuffer capacity must be greater than zero");

    std::array<std::optional<T>, Capacity> data_;
    std::size_t head_ = 0;
    std::size_t size_ = 0;
};


int main(){
    RingBuffer<std::string, 3> buffer;

    buffer.push("A");
    buffer.push("B");
    buffer.push("C");

    if (buffer.front() != "A" || buffer.back() != "C") {
        throw std::runtime_error("front() or back() returned an unexpected value");
    }

    buffer.push("D");
    if (buffer.front() != "B" || buffer.back() != "D") {
        throw std::runtime_error("oldest value was not overwritten");
    }

    while (const auto value = buffer.pop()) {
        std::cout << *value << '\n';
    }

    if (buffer.pop().has_value()) {
        throw std::runtime_error("pop() should return nullopt for an empty buffer");
    }

    try {
        buffer.front();
    } catch (const std::out_of_range& error) {
        std::cout << error.what() << '\n';
    }
}