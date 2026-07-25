#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: missing directory argument." << std::endl;
        return 1;
    }

    fs::path target = argv[1];
    if (!fs::exists(target)) {
        std::cerr << "Error: directory does not exist." << std::endl;
        return 1;
    }
    if (!fs::is_directory(target)) {
        std::cerr << "Error: path is not a directory." << std::endl;
        return 1;
    }

    std::size_t file_count = 0;
    std::size_t directory_count = 0;
    std::uintmax_t total_size = 0;
    fs::path largest_file;
    std::uintmax_t largest_size = 0;

    constexpr std::size_t kMaxExtensions = 1024;
    std::string extensions[kMaxExtensions];
    std::size_t counts[kMaxExtensions];
    std::size_t extension_count = 0;

    auto add_extension = [&](const std::string& ext) {
        for (std::size_t i = 0; i < extension_count; ++i) {
            if (extensions[i] == ext) {
                ++counts[i];
                return;
            }
        }
        if (extension_count < kMaxExtensions) {
            extensions[extension_count] = ext;
            counts[extension_count] = 1;
            ++extension_count;
        }
    };

    auto record_file = [&](const fs::path& path) {
        ++file_count;
        const auto size = fs::file_size(path);
        total_size += size;
        if (size > largest_size) {
            largest_size = size;
            largest_file = path;
        }

        const std::string ext = path.extension().string();
        if (!ext.empty()) {
            add_extension(ext);
        }
    };

    for (const auto& entry : fs::directory_iterator(target)) {
        if (entry.is_directory()) {
            ++directory_count;
        } else if (entry.is_regular_file()) {
            record_file(entry.path());
        }
    }

    for (const auto& entry : fs::recursive_directory_iterator(target)) {
        if (entry.path() == target) {
            continue;
        }
        if (entry.path().parent_path() == target) {
            continue;
        }

        if (entry.is_directory()) {
            ++directory_count;
        } else if (entry.is_regular_file()) {
            record_file(entry.path());
        }
    }

    std::cout << "Directory : " << target << '\n';
    std::cout << "Files      : " << file_count << '\n';
    std::cout << "Directories: " << directory_count << '\n';
    std::cout << "Total Size : " << total_size << " bytes\n\n";
    std::cout << "Largest File\n";
    std::cout << "------------\n";
    if (file_count > 0) {
        std::cout << "path : " << largest_file << '\n';
        std::cout << "size : " << largest_size << " bytes\n";
    } else {
        std::cout << "path : (none)\n";
        std::cout << "size : 0 bytes\n";
    }

    if (extension_count > 0) {
        std::cout << "\nExtensions\n";
        std::cout << "----------\n";
        for (std::size_t i = 0; i < extension_count; ++i) {
            std::cout << extensions[i] << " : " << counts[i] << '\n';
        }
    }

    return 0;
}
