#include<iostream>

std::vector<uint8_t> buffer_;

std::vector<uint8_t> &getbuffer() { return buffer_; }
int main() {
    std::vector<uint8_t> a(1022);
    std::cout << "Buffer size: " << a.size() << std::endl;
    
}