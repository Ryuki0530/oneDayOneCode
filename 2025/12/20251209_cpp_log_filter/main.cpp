#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <logfile> <loglevel>" << std::endl;
        return 1;
    }

    std::string logfile = argv[1];
    std::string loglevel = argv[2];
    std::ifstream file(logfile);
    std::string log_prefix = "[" + loglevel + "]";

    std::ifstream infile(logfile);
    if (!file.is_open()) {
        std::cout << "Failed to open file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find(log_prefix) == 0) {
            std::cout << line << std::endl;
        }
    }

    file.close();

    return 0;
}