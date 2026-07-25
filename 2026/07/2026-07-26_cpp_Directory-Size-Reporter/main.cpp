#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
	const fs::path target = (argc >= 2) ? fs::path(argv[1]) : fs::current_path();

	if (!fs::exists(target) || !fs::is_directory(target)) {
		std::cerr << "Invalid directory: " << target.string() << '\n';
		return 1;
	}

	struct FileInfo {
		std::string name;
		std::uintmax_t size;
	};

	std::vector<FileInfo> files;
	for (const auto& entry : fs::directory_iterator(target)) {
		if (!entry.is_regular_file()) {
			continue;
		}

		files.push_back({entry.path().filename().string(), entry.file_size()});
	}

	std::sort(files.begin(), files.end(), [](const FileInfo& a, const FileInfo& b) {
		if (a.size != b.size) {
			return a.size < b.size;
		}
		return a.name < b.name;
	});

	std::uintmax_t totalSize = 0;
	for (const auto& file : files) {
		std::cout << file.name << " : " << file.size << " Byte\n";
		totalSize += file.size;
	}

	std::cout << '\n';
	std::cout << "Files : " << files.size() << '\n';
	std::cout << "Total : " << totalSize << " Byte\n";

	return 0;
}
