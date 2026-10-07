#include <iostream>
#include <vector>

// this experiment reinforces the concept: *C++ copies by default*
// in JS, when you pass an array/object into a function,
// the function is accessing the array/object directly
// in C++ each parameter receives a _copy_ of the argument
// this is called "pass by value"

// 3: a _copy_ of numbers is passed in to our function
void changeFirstElement(std::vector<int> numbers) {
    // 4: the first element in the copied vector is changed from 1 to 99
    numbers[0] = 99;
} // 5: at the closing brace, when the function ends, the copy is destroyed, along with the 99


int main () {
    // 1: we declare vector numbers and assign 1, 2, 3 to it
    std::vector<int> numbers = {1, 2, 3};

    // 2: we call our function, a copy of our data 1, 2, 3 is made and passed in
    changeFirstElement(numbers);

    // 6: the program prints the element as it appears inside this function, main - still a 1
    // note that with JavaScript, we would get 99, as the original array has been mutated
    std::cout << "first element: " << numbers[0]; // 1, not 99

    // if we wanted to print 99, we would have to return the new vector in the function
    // and then store it in a new variable when we call changeFirstElement(numbers);
    // std::vector<int> numbersMutant = changeFirstElement(numbers); - then print numbersMutant[0]
    // but note the cost: the data has to be copied on the way into the function

    return 0;
}

// so what? well consider writeFile(header, samples, outputPath)
// when we call it it copies all of this data - header, samples, outputPath
// reads it once, then bins it. samples would be particularly costly
// scale this up with more channels, bits, samples, duration...
// we're copying and throwing away a lot of data
