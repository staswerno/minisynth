#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <exception>
#include "wav_file.h"
#include "tone_generator.h"

void printUsage() {
    std::cerr << "correct usage: minisynth [-f|--frequency HZ] [-d|--duration SECONDS] [-o|--output PATH]\n";
}


// program starts here
// main holds our default "choices" (frequency, amplitude etc)
// runs the functions (passing values between them) necessary for the desired result of the program 
int main(int argc, char* argv[]) {
    float durationSeconds = 3; //seconds
    float frequency = 528; // Hz
    int amplitude = 10000; // peak amplitude, must stay within int16_t range (±32767)
    std::string outputPath = "tone.wav";
    // declaring all on one line, e.g. "bool a, b = false;", would leave a uninitialised
    bool seenFrequency = false;
    bool seenDuration = false;
    bool seenOutput = false;

    // read options and check for errors
    for (int i = 1; i < argc; i++) {
        // argv[i] is a char*: a memory address, not the text itself
        // == on two char*s compares addresses, so it's always false for our options
        // a std::string compares contents with ==, so we convert first
        std::string argument = argv[i];
        
        if (argument == "-f" || argument == "--frequency") {
            // check for duplicates
            if (seenFrequency) {
                std::cerr << argument << " given more than once" << "\n";
                printUsage();
                return 1;
            }
            // i + 1 < argc is true only if there is a next argument
            else if (i+1 < argc && argv[i + 1][0] != '-') {
                std::string frequencyValue = argv[i + 1]; 
                try {
                    std::size_t charactersRead = 0; // std::size_t is the standard unsigned type for sizes and counts
                    frequency = std::stof(frequencyValue, &charactersRead); // see README: "why std::stof takes an address"
                    if (charactersRead != frequencyValue.size()) {
                        std::cerr << "invalid frequency: " << frequencyValue << "\n"; 
                        printUsage();
                        return 1;
                    }
                }
                // std::exception is the standard library general error type
                catch (const std::exception&) {
                    // runs only if stof threw
                    std::cerr << "invalid frequency: " << frequencyValue << "\n"; 
                    printUsage();
                    return 1;
                }
                seenFrequency = true;
                i++;
            }
            else {
                std::cerr << "missing value for " << argument << "\n"; // always use cerr for error messages so errors go to error stream
                printUsage();
                return 1; // exit the program with an error code. nothing more runs, so nothing is written
            }
        } 
        else if (argument == "-d" || argument == "--duration") {
            if (seenDuration) {
                std::cerr << argument << " given more than once" << "\n";
                printUsage();
                return 1;
            }
            else if (i+1 < argc && argv[i + 1][0] != '-') {
                std::string durationValue = argv[i + 1]; 
                try {
                    std::size_t charactersRead = 0;
                    durationSeconds = std::stof(durationValue, &charactersRead);
                    if (charactersRead != durationValue.size()) {
                        std::cerr << "invalid duration: " << durationValue << "\n"; 
                        printUsage();
                        return 1;
                    }
                }
                catch (const std::exception&) {
                    std::cerr << "invalid duration: " << durationValue << "\n"; 
                    printUsage();
                    return 1;
                }
                seenDuration = true;
                i++;
            }
            else {
                std::cerr << "missing value for " << argument << "\n";
                printUsage();
                return 1;
            }
        } 
        else if (argument == "-o" || argument == "--output") {
            if (seenOutput) {
                std::cerr << argument << " given more than once" << "\n";
                printUsage();
                return 1;
            }
            else if (i+1 < argc && argv[i + 1][0] != '-') {
                std::string outputValue = argv[i + 1]; 
                outputPath = outputValue;
                seenOutput = true;
                i++;
            }
            else {
                std::cerr << "missing value for " << argument << "\n";
                printUsage();
                return 1;
            }
        } 
        else {
            if (argument[0] == '-') {
                std::cerr << "unknown option: " << argument << "\n";
            }
            else {
                std::cerr << "value without an option: " << argument << "\n";
            }
            printUsage();
            return 1;
            
        }
    }

    // durationSeconds: we are using an existing variable, hence no type declaration
    // createWavHeader() returns the *value* of its own "header" variable (which then disappears),
    // and we store it in a NEW "header" variable here, for use in the following functions
    WavHeader header = createWavHeader(durationSeconds);
    std::vector<int16_t> samples = generateTone(durationSeconds, header.sampleRate, frequency, amplitude);
    writeFile(header, samples, outputPath);

    return 0;
}
