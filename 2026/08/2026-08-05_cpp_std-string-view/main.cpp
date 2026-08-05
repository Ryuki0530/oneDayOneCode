#include <iostream>
#include <string_view>

void parseLogLine(std::string_view log) {
    size_t colonPos = log.find(':');
    
    if (colonPos == std::string_view::npos) {
        std::cout << "Level   : UNKNOWN\n";
        std::cout << "Message : " << log << "\n";
        return;
    }
    
    std::string_view level = log.substr(0, colonPos);
    std::string_view message = log.substr(colonPos + 1);    
    bool isValid = (level == "INFO" || level == "WARN" || level == "ERROR");
    
    if (!isValid) {
        std::cout << "Level   : UNKNOWN\n";
    } else {
        std::cout << "Level   : " << level << "\n";
    }
    
    std::cout << "Message : " << message << "\n";
}

int main() {
    parseLogLine("INFO:Robot started");
    std::cout << "\n";
    parseLogLine("ERROR:Motor failed");
    std::cout << "\n";
    parseLogLine("WARN:Low battery");
    std::cout << "\n";
    parseLogLine("DEBUG:Unknown level");
    
    return 0;
}