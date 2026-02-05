#include <iostream>

class HardwareInfo {
public:
    HardwareInfo() {
        // Constructor implementation
    }

    std::string showCPUInfo() {
        // Method to show CPU information
    }
    std::string showMemoryInfo() {
        // Method to show Memory information
    }
    std::string showDiskInfo() {
        // Method to show Disk information
    }
    std::string showNetworkInfo() {
        // Method to show Network information
    }
    std::string showPCIInfo() {
        // Method to show PCI information
    }
    std::string showAllInfo() {
        // Method to show all hardware information
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2){
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    HardwareInfo hwInfo = HardwareInfo();
    std::string cmd = argv[1];

    if (cmd == "cpu") {
        std::cout << hwInfo.showCPUInfo() << std::endl;
    } else if (cmd == "memory") {
        std::cout << hwInfo.showMemoryInfo() << std::endl;
    } else if (cmd == "disk") {
        std::cout << hwInfo.showDiskInfo() << std::endl;
    } else if (cmd == "network") {
        std::cout << hwInfo.showNetworkInfo() << std::endl;
    } else if (cmd == "pci") {
        std::cout << hwInfo.showPCIInfo() << std::endl;
    } else if (cmd == "all") {
        std::cout << hwInfo.showAllInfo() << std::endl;
    } else {
        std::cerr << "Unknown command: " << cmd << std::endl;
        return 1;
    }

}