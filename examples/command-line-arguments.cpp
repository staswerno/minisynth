#include <iostream>

void printArguments (int argc, char* argv[]) {

    std::cout << "argc: " << argc << "\n"; // in our example, prints 4 (program name + 3 additional args)

    // C-style arrays have no .length, so argc gives the length of argv
    for (int i = 0; i < argc; i++) {

        // in our example, prints:
        // argv 0:./examples/command-line-arguments
        // argv 1:hello
        // argv 2:440
        // argv 3:2.5
        std::cout << "argv " << i << ":" << argv[i] << "\n"; 
    }

}

// main can take 2 parameters, which give it everything you typed, starting with the program name (the command used to run the program)
// argc -> argument count -> how many arguments have been passed in (_including_ the program name)
// argv -> argument values -> held as text (even numbers). program name is always argv[0]
// argv[2] is a char*: a char* points to the first character of “440”, and the text runs until a hidden \0
// argv is a char**: a char** points to the list of char*s, one per argument
int main (int argc, char* argv[]) {

    printArguments(argc, argv); // for e.g. run ./examples/command-line-arguments hello 440 2.5

}

