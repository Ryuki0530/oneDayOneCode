#include <array>
#include <cstddef>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

template <typename T, std::size_t Capacity>
class RingBuffer {
public:
    void push(T value){
    };
    std::optional<T> pop(){
    };

    const T& front() const{
    };
    const T& back() const{
    };

    bool empty() const{
    };
    bool full() const{
    };
    std::size_t size() const{
    };

private:
    std::array<std::optional<T>, Capacity> data_;
    std::size_t head_ = 0;
    std::size_t size_ = 0;
};


int main(){

}