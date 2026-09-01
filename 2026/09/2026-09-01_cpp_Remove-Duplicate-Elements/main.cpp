#include <iostream>
#include <vector>
#include <algorithm>

int main(){
    std::vector<int> nums = {1, 2, 2, 3, 4, 4, 5};
    
    std::sort(nums.begin(), nums.end());
    auto it = std::unique(nums.begin(), nums.end());
    nums.erase(it, nums.end());
    
    for (const auto& num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    return 0;
}