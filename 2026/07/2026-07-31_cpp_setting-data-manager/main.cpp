#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

class TemporaryFile {
public:
	explicit TemporaryFile(const std::string& path)
		: path_(path), fd_(-1) {
		fd_ = ::open(path_.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd_ == -1) {
			throw std::runtime_error(std::string("open failed: ") + std::strerror(errno));
		}
	}

	~TemporaryFile() {
		cleanup();
	}

	TemporaryFile(const TemporaryFile&) = delete;
	TemporaryFile& operator=(const TemporaryFile&) = delete;

	TemporaryFile(TemporaryFile&& other) noexcept
		: path_(std::move(other.path_)), fd_(other.fd_) {
		other.fd_ = -1;
	}

	TemporaryFile& operator=(TemporaryFile&& other) noexcept {
		if (this != &other) {
			cleanup();
			path_ = std::move(other.path_);
			fd_ = other.fd_;
			other.fd_ = -1;
		}
		return *this;
	}

	void writeText(const std::string& text) {
		std::size_t totalWritten = 0;
		while (totalWritten < text.size()) {
			const ssize_t written = ::write(fd_, text.data() + totalWritten, text.size() - totalWritten);
			if (written == -1) {
				if (errno == EINTR) {
					continue;
				}
				throw std::runtime_error(std::string("write failed: ") + std::strerror(errno));
			}
			totalWritten += static_cast<std::size_t>(written);
		}
	}

	std::string readText() {
		if (::lseek(fd_, 0, SEEK_SET) == -1) {
			throw std::runtime_error(std::string("lseek failed: ") + std::strerror(errno));
		}

		std::string result;
		char buffer[128];
		while (true) {
			const ssize_t n = ::read(fd_, buffer, sizeof(buffer));
			if (n == -1) {
				if (errno == EINTR) {
					continue;
				}
				throw std::runtime_error(std::string("read failed: ") + std::strerror(errno));
			}
			if (n == 0) {
				break;
			}
			result.append(buffer, static_cast<std::size_t>(n));
		}
		return result;
	}

private:
	void cleanup() {
		if (fd_ != -1) {
			::close(fd_);
			::unlink(path_.c_str());
			fd_ = -1;
		}
	}

	std::string path_;
	int fd_;
};

int main() {
	const std::string path = "/tmp/one_day_one_code.txt";

	{
		TemporaryFile file(path);
		file.writeText("Hello system call!\n");
		file.writeText("Managed by RAII.\n");
		std::cout << file.readText();
	}

	if (::access(path.c_str(), F_OK) == -1) {
		std::cout << "Temporary file was deleted." << std::endl;
	}

	return 0;
}
