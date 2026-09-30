// includes library for input/output (essentially console printing)
// preprocessor runs before compilation, scanning for lines starting with #
// #include <iostream> essentially means "find iostream file, paste entire contents here
#include <iostream>
// include fixed-width integer types to test struct padding
#include <cstdint>

// comment out/in pragma pack to see effect on struct padding
#pragma pack(push, 1)
struct PaddingTest {
    uint8_t a;
    uint32_t b;
};
#pragma pack(pop)

// every c++ program needs exactly one main function
int main() {
    // character output = console printing tool
    // std = standard namespace. cout lives here
    // << is the stream insertion operator used to send data to the output stream
    std::cout << "hello, world!" << "\n";
    // test struct padding
    std::cout << "size of struct in bytes: " << sizeof(PaddingTest) << "\n"; // output 8 without pragma, 5 with pragma
    // exit code - 0 means successful execution
    return 0;
}
