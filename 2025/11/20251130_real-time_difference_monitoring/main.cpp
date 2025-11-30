#include <iostream>
#include <string>

void check_diff(const std::string& prev, const std::string& curr){
    if(curr == prev) return;

    size_t sa = prev.size();
    size_t sb = curr.size();

    // 最長共通接頭辞
    size_t p = 0;
    while(p < sa && p < sb && prev[p] == curr[p]) ++p;

    // 最長共通接尾辞
    size_t s = 0;
    while(s < sa - p && s < sb - p &&
          prev[sa - 1 - s] == curr[sb - 1 - s]){
        ++s;
    }

    // 中間部分の切り出し
    std::string old_mid = prev.substr(p, sa - p - s);
    std::string new_mid = curr.substr(p, sb - p - s);

    // 出力
    if(!old_mid.empty() && new_mid.empty()){
        std::cout << "- " << old_mid << '\n';
    }
    else if(old_mid.empty() && !new_mid.empty()){
        std::cout << "+ " << new_mid << '\n';
    }
    else{
        std::cout << "- " << old_mid << '\n';
        std::cout << "+ " << new_mid << '\n';
    }
}

int main(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // 直前の入力の退避用
    std::string prev;

    // 最初の一行
    if(!std::getline(std::cin, prev) || prev.empty()) return 0;

    // 現在の入力用
    std::string curr;

    // メインループ
    while(std::getline(std::cin, curr)){
        if(curr.empty()) break;

        check_diff(prev, curr);

        prev = curr;
    }

    return 0;
}