// includes library for input/output (essentially console printing)
// preprocessor runs before compilation, scanning for lines starting with #
// #include <iostream> essentially means "find iostream file, paste entire contents here
#include <iostream>

// every c++ program needs exactly one main function
int main() {
    // character output = console printing tool
    // std = standard namespace. cout lives here
    // << is the stream insertion operator used to send data to the output stream
    std::cout << "hello";
    // exit code - 0 means successful execution
    return 0;
}
