#include <iostream>
#include <string>
#include <windows.h>
#include <curl/curl.h>
#include "json.hpp"

#define API_URL "https://api.thedogapi.com/v1/images/search"
using json = nlohmann::json;

//　prototypes
std::string get_json_data(std::string url);
std::string image_url(std::string json_data);
std::string get_dog_image(std::string image_url);
bool view_image(std::string image_path);


//================
// get_json_data
//================
size_t WriteToString(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append((char*)contents, totalSize);
    return totalSize;
}
std::string get_json_data(std::string url) {
    
    
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "CURL initialization failed!" << std::endl;
        return "";
    }
    std::string response_data;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
        return "";
    }
    return response_data;
}
//================
// image_url
//================
std::string image_url(std::string json_data) {
    try {
        auto j = json::parse(json_data);
        if (j.is_array() && !j.empty() && j[0].contains("url")) {
            return j[0]["url"].get<std::string>();
        } else {
            std::cerr << "Invalid JSON structure!" << std::endl;
            return "";
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return "";
    }
}

//================
// get_dog_image
//================
std::string get_dog_image(std::string image_url) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "CURL initialization failed!" << std::endl;
        return "";
    }
    std::string image_path = "dog_image.jpg";
    FILE* fp = fopen(image_path.c_str(), "wb");
    if (!fp) {
        std::cerr << "Failed to open file for writing!" << std::endl;
        curl_easy_cleanup(curl);
        return "";
    }

    curl_easy_setopt(curl, CURLOPT_URL, image_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    CURLcode res = curl_easy_perform(curl);
    fclose(fp);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
        return "";
    }
    return image_path;
}

// ================
// view_image
// ================
bool view_image(std::string image_path) {
    HINSTANCE result = ShellExecuteA(NULL, "open", image_path.c_str(), NULL, NULL, SW_SHOWNORMAL);
    if ((intptr_t)result <= 32) {
        std::cerr << "Failed to open image!" << std::endl;
        return false;
    }
    return true;
}


int main() {
    const std::string api_url = API_URL;
    std::cout << "===========================================================\n" << std::endl;
    std::cout << "Hello This is Dog API's wrapper" << std::endl;
    std::cout << "Dog API URL: " << api_url << std::endl;
    std::cout << "\n===========================================================\n" << std::endl;
    std::cout << "Connecting to Dog API..." << std::endl;

    // APIを使用してJSONデータを取得
    std::string json_data = get_json_data(api_url);
    if (json_data.empty()) {
        std::cerr << "Failed to retrieve JSON data from API!" << std::endl;
        return 1;
    }
    std::cout << "Received response: " << json_data << std::endl;
    // JSONデータを解析して画像URLを取得
    std::string img_url = image_url(json_data);
    if (img_url.empty()) {
        std::cerr << "Failed to extract image URL from JSON data!" << std::endl;
        return 1;
    }
    // 画像URLを使用して画像をダウンロードして表示
    std::cout << "Downloading Dog Image from: " << img_url << "..."<< std::endl;
    std::string img_path = get_dog_image(img_url);
    if (img_path.empty()) {
        std::cerr << "Failed to download dog image!" << std::endl;
        return 1;
    }
    std::cout << "Image saved to: " << img_path << std::endl;
    std::cout << "Opening image..." << std::endl;
    if (!view_image(img_path)) {
        return 1;
    }
    std::cout << "Enjoy Dog Image! \nGoodbye! Happy Hacking!!" << std::endl;
    return 0;
}