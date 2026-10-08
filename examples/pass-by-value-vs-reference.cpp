#include <iostream>
#include <vector>

// this experiment reinforces the concept: *C++ copies by default*
// in JS, when you pass an array/object into a function,
// the function is accessing the array/object directly
// in C++ each parameter receives a _copy_ of the argument
// this is called "pass by value"
// in C++, we can access objects directly using references

// 3A: a _copy_ of numbersCopy is passed in to our function
void changeFirstElementCopy(std::vector<int> vectorCopy) {
    // 4A: the first element in the copied vector is changed from 1 to 99
    vectorCopy[0] = 99;
} // 5A: at the closing brace, when the function ends, the copy is destroyed, along with the 99

// 3B: the parameter vectorReference becomes a second name for main's numbersReference vector
// (we can also use the same name, numbersReference: what's important is
// that the compiler sees it as a second name for the original vector)
// the parameter becomes a reference by appending & to the type
void changeFirstElementReference(std::vector<int>& vectorReference) {
    // 4B: the first element in the original vector in main is changed from 1 to 99
    vectorReference[0] = 99;
} // 5B: the reference ends, so the second name disappears,
  // but the vector lives in main, so it survives, along with the 99

int main () {
    // 1A: we declare vector numbersCopy and assign 1, 2, 3 to it
    std::vector<int> numbersCopy = {1, 2, 3};

    // 1B: we declare vector numbersReference and assign 1, 2, 3 to it
    std::vector<int> numbersReference = {1, 2, 3};

    // 2A: we call our function, a copy of our data 1, 2, 3 is made and passed in
    changeFirstElementCopy(numbersCopy);

    // 2B: we call our function, no copy is made
    changeFirstElementReference(numbersReference);

    // 6A: the program prints the element as it appears inside this function, main - still a 1
    // note that with JavaScript, we would get 99, as the original array has been mutated
    std::cout << "numbersCopy first element: " << numbersCopy[0] << "\n"; // 1, not 99

    // 6B: the program prints the element as it appears inside this function, main - 99
    std::cout << "numbersReference first element: " << numbersReference[0] << "\n"; // 99

    // without the reference, if we wanted to print 99, we would have to return the new vector in the function
    // and then store it in a new variable when we call changeFirstElementCopy(numbersCopy);
    // std::vector<int> numbersMutant = changeFirstElementCopy(numbersCopy); - then print numbersMutant[0]
    // but note the cost: the data has to be copied on the way into the function

    return 0;
}

// so what? well consider writeFile(header, samples, outputPath)
// when we call it it copies all of this data - header, samples, outputPath
// reads it once, then bins it. samples would be particularly costly
// scale this up with more channels, bits, samples, duration...
// we're copying and throwing away a lot of data

// note on choosing parameter types:
// small, simple values (`int`, `float`, `uint32_t`): pass by value
// copying costs about the same as a reference, and it's simpler
// bigger things only a function reads: pass by *const* reference (see wav_file.cpp)
// things the function is meant to change: pass by plain reference

// warning regarding reference: this is not quite the same thing as 
// the use of &header in writeFile. that is the address of the variable "header"
// & after a type = reference; & before a variable = address
