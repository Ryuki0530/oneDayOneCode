#include <cstddef>
#include <iostream>
#include <stdexcept>

class CircularBuffer {
public:
	explicit CircularBuffer(std::size_t capacity)
		: data_(capacity == 0 ? nullptr : new int[capacity]),
		  capacity_(capacity),
		  head_(0),
		  tail_(0),
		  size_(0) {
		if (capacity == 0) {
			throw std::invalid_argument("capacity must be at least 1");
		}
	}

	~CircularBuffer() {
		delete[] data_;
	}

	CircularBuffer(const CircularBuffer&) = delete;
	CircularBuffer& operator=(const CircularBuffer&) = delete;

	bool push(int value) {
		if (full()) {
			return false;
		}

		data_[tail_] = value;
		tail_ = (tail_ + 1) % capacity_;
		++size_;
		return true;
	}

	bool pop(int& value) {
		if (empty()) {
			return false;
		}

		value = data_[head_];
		head_ = (head_ + 1) % capacity_;
		--size_;
		return true;
	}

	std::size_t size() const {
		return size_;
	}

	bool empty() const {
		return size_ == 0;
	}

	bool full() const {
		return size_ == capacity_;
	}

private:
	int* data_;
	std::size_t capacity_;
	std::size_t head_;
	std::size_t tail_;
	std::size_t size_;
};

int main() {
	CircularBuffer buffer(3);
	std::cout << std::boolalpha;

	std::cout << "push(10): " << buffer.push(10) << '\n';
	std::cout << "push(20): " << buffer.push(20) << '\n';
	std::cout << "push(30): " << buffer.push(30) << '\n';
	std::cout << "push(40): " << buffer.push(40) << '\n';

	int value;
	if (buffer.pop(value)) {
		std::cout << "pop(): true, value = " << value << '\n';
	} else {
		std::cout << "pop(): false\n";
	}
	if (buffer.pop(value)) {
		std::cout << "pop(): true, value = " << value << '\n';
	} else {
		std::cout << "pop(): false\n";
	}

	std::cout << "push(40): " << buffer.push(40) << '\n';
	std::cout << "push(50): " << buffer.push(50) << '\n';

	while (buffer.pop(value)) {
		std::cout << "pop(): true, value = " << value << '\n';
	}
	std::cout << "pop(): false\n";

	return 0;
}


