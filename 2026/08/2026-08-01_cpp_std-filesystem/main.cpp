#include <cstdio>
#include <filesystem>
#include <string>
#include <iostream>

int main() {
    std::filesystem::path current_directory_path = ".";
    for (const auto& entry : std::filesystem::directory_iterator(current_directory_path)) {
        if (std::filesystem::is_regular_file(entry.status())) {
            printf(
                "%s (%llu bytes)\n", 
                entry.path().filename().string().c_str(), 
                static_cast<unsigned long long>(
                    std::filesystem::file_size(entry.path())
                )
            );
        }
    }
    return 0;
}