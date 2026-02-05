#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>

class HardwareInfo {
private:
    std::string readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return "";
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void listDir(const std::string& path, bool recursive = false) {
        DIR* dir = opendir(path.c_str());
        if (!dir) return;

        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_name[0] == '.') continue;
            std::cout << "  " << entry->d_name << "\n";
        }
        closedir(dir);
    }


public:
    HardwareInfo() {
        // Constructor implementation

    }

    std::string showCPUInfo() {
        // Method to show CPU information
        return readFile("/proc/cpuinfo");
    }

    std::string showMemoryInfo() {
        // Method to show Memory information
        return readFile("/proc/meminfo");
    }
    
    std::string showDiskInfo() {
        // Method to show Disk information
        return readFile("/proc/partitions");
    }
    
    std::string showNetworkInfo() {
        // Method to show Network information
        return readFile("/proc/net/dev");
    }
    
    std::string showPCIInfo() {
        // Method to show PCI information
        return readFile("/proc/bus/pci/devices");
    }
    
    std::string showAllInfo() {
        // Method to show all hardware information
        std::stringstream allInfo;
        allInfo << "CPU Info:\n" << showCPUInfo() << "\n";
        allInfo << "Memory Info:\n" << showMemoryInfo() << "\n";
        allInfo << "Disk Info:\n" << showDiskInfo() << "\n";
        allInfo << "Network Info:\n" << showNetworkInfo() << "\n";
        allInfo << "PCI Info:\n" << showPCIInfo() << "\n";
        return allInfo.str();
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