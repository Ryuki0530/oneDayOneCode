#include<iostream>
#include<string>
#include<windows.h>

#define API_URL "https://api.thedogapi.com/v1/images/search"

int main() {
    const std::string api_url = API_URL;
    std::cout << "===========================================================\n" << std::endl;
    std::cout << "Hello This is Dog API's wrapper" << std::endl;
    std::cout << "Dog API URL: " << api_url << std::endl;
    std::cout << "\n===========================================================\n" << std::endl;
    std::cout << "Connecting to Dog API..." << std::endl;
    return 0;
}