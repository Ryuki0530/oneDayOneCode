#include <charconv>
#include <string_view>
#include <system_error>
#include <iostream>
#include <array>

int from_chars(std::string_view str, int& value) {
    const char* begin = str.data();
    const char* end = str.data() + str.size();
    auto result = std::from_chars(begin, end, value);

    if (result.ec == std::errc::invalid_argument) {
        return 2;
    }
    if (result.ec == std::errc::result_out_of_range) {
        return 3;
    }
    if (result.ec == std::errc() && result.ptr != end) {
        return 1;
    }
    return 0;
}

int main() {

    std::array<std::string_view, 4> test_cases = {
        "123",
        "-45",
        "12abc",
        "999999999999999999999",
    };
 
    for (const auto& sample: test_cases) {
        int value;
        int result = from_chars(sample, value);
        switch (result) {
            case 0:
                std::cout << "成功: " << value << std::endl;
                break;
            case 1:
                std::cout << "途中まで成功: " << value << std::endl;
                break;
            case 2:
                std::cout << "変換失敗: " << value << std::endl;
                break;
            case 3:
                std::cout << "範囲超過: " << value << std::endl;
                break;
        }

        // 次ループのために value を0にリセット
        value = 0;
    }

    return 0;
}