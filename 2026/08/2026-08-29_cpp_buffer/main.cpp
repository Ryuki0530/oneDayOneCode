#include <cstddef>
#include <iostream>
#include <utility>

class Buffer {
public:
    explicit Buffer(std::size_t size)
        : data_(new int[size]{}), size_(size) {}

    ~Buffer() {
        delete[] data_;
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    int& operator[](std::size_t index) {
        return data_[index];
    }

    const int& operator[](std::size_t index) const {
        return data_[index];
    }

    std::size_t size() const {
        return size_;
    }

private:
    int* data_;
    std::size_t size_;
};

int main() {
    Buffer a(3);

    a[0] = 10;
    a[1] = 20;
    a[2] = 30;

    Buffer b(std::move(a));

    std::cout << "a.size(): " << a.size() << '\n';
    std::cout << "b.size(): " << b.size() << '\n';

    for (std::size_t i = 0; i < b.size(); ++i) {
        std::cout << b[i] << '\n';
    }

    Buffer c(5);
    c = std::move(b);

    std::cout << "b.size(): " << b.size() << '\n';
    std::cout << "c.size(): " << c.size() << '\n';

    return 0;
}
