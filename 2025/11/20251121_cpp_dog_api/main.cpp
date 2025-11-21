#include<iostream>
#include<string>
#include<windows.h>

#define API_URL "https://api.thedogapi.com/v1/images/search"

//　prototypes
std::string get_json_data(std::string url);
std::string image_url(std::string json_data);
std::string get_dog_image(std::string image_url);
bool view_image(std::string image_path);

int main() {
    const std::string api_url = API_URL;
    std::cout << "===========================================================\n" << std::endl;
    std::cout << "Hello This is Dog API's wrapper" << std::endl;
    std::cout << "Dog API URL: " << api_url << std::endl;
    std::cout << "\n===========================================================\n" << std::endl;
    std::cout << "Connecting to Dog API..." << std::endl;

    // APIを使用してJSONデータを取得
    std::string json_data = get_json_data(api_url);
    // JSONデータを解析して画像URLを取得
    std::string img_url = image_url(json_data);
    // 画像URLを使用して画像をダウンロードして表示
    std::string img_path = get_dog_image(img_url);
    view_image(img_path);

    return 0;
}