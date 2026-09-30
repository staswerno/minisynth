// include library for input/output (essentially console printing)
// see test.cpp for more detailed understanding
#include <iostream>
// include fixed-width integer types
// wav files require fixed-width integer types for proper header formatting
#include <cstdint>

// every c++ program needs exactly one main function
int main() {
    // uint = unsigned integer to represent only non-negative values (0-255 ∴ 256)
    // output of uint32_t = 4 (size of int in bytes) - in this instance guaranteed, unlike "int"
    // note: byte-width tells you how much space something takes, but not what it means
    std::cout << sizeof(int) << "\n";
    std::cout << sizeof(uint32_t) << "\n";
    std::cout << sizeof(uint16_t) << "\n";
    std::cout << sizeof(uint8_t) << "\n";
    return 0;
}

