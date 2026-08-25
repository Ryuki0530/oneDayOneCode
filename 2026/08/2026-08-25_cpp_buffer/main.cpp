#include <cstddef>
#include <iostream>
#include <utility>

class Buffer {
private:
	int* data_;
	std::size_t size_;

public:
	Buffer(std::size_t size)
		: data_(size ? new int[size]() : nullptr), size_(size) {}

	~Buffer() {
		delete[] data_;
	}

	Buffer(const Buffer& other)
		: data_(other.size_ ? new int[other.size_] : nullptr), size_(other.size_) {
		for (std::size_t i = 0; i < size_; ++i) {
			data_[i] = other.data_[i];
		}
	}

	Buffer& operator=(const Buffer& other) {
		if (this == &other) {
			return *this;
		}

		int* new_data = other.size_ ? new int[other.size_] : nullptr;
		for (std::size_t i = 0; i < other.size_; ++i) {
			new_data[i] = other.data_[i];
		}

		delete[] data_;
		data_ = new_data;
		size_ = other.size_;
		return *this;
	}

	Buffer(Buffer&& other) noexcept
		: data_(other.data_), size_(other.size_) {
		other.data_ = nullptr;
		other.size_ = 0;
	}

	Buffer& operator=(Buffer&& other) noexcept {
		if (this == &other) {
			return *this;
		}

		delete[] data_;
		data_ = other.data_;
		size_ = other.size_;

		other.data_ = nullptr;
		other.size_ = 0;
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
};

int main()
{
	Buffer a(3);

	a[0] = 10;
	a[1] = 20;
	a[2] = 30;

	Buffer b = a;

	b[0] = 999;

	std::cout << "a[0] = " << a[0] << '\n';
	std::cout << "b[0] = " << b[0] << '\n';

	Buffer c(1);
	c = a;

	std::cout << "c[1] = " << c[1] << '\n';

	Buffer d = std::move(a);

	std::cout << "d size = " << d.size() << '\n';
	std::cout << "a size = " << a.size() << '\n';

	Buffer e(100);
	e = std::move(b);

	std::cout << "e[0] = " << e[0] << '\n';
	std::cout << "b size = " << b.size() << '\n';
}
