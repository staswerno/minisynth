#include <iostream>
#include <exception>
#include <string>
#include <cctype>

#include "options.h"

void printUsage() {
    std::cerr << "correct usage: minisynth [-f|--frequency HZ] [-d|--duration SECONDS] [-o|--output PATH] [-w|--waveform NAME]\n";
}

bool parseOptions(int argc, char* argv[], Options& options) {
    // declaring all on one line, e.g. "bool a, b = false;", would leave a uninitialised
    bool seenFrequency = false;
    bool seenDuration = false;
    bool seenOutput = false;
    bool seenWaveform = false;

    // read options and check for errors
    for (int i = 1; i < argc; i++) {
        // argv[i] is a char*: a memory address, not the text itself
        // == on two char*s compares addresses, so it's always false for our options
        // a std::string compares contents with ==, so we convert first
        std::string argument = argv[i];

        // checks refactored from if/else tree in tag v2-cli-before-refactor to guards
        if (argument == "-f" || argument == "--frequency") {
            // check for duplicates
            if (seenFrequency) {
                std::cerr << argument << " given more than once" << "\n"; // always use cerr for error messages so errors go to error stream
                printUsage();
                return false;
            }
            // check for negative values (a digit follows the dash)
            if (i+1 < argc && argv[i + 1][0] == '-' && std::isdigit(argv[i + 1][1])) {
                std::cerr << "frequency must be between 1 and 20000 Hz: " << argv[i + 1] << "\n";
                printUsage();
                return false;
            }
            // i + 1 < argc is true only if there is a next argument. see README for evaluation explanation
            if (!(i+1 < argc && argv[i + 1][0] != '-')) {
                std::cerr << "missing value for " << argument << "\n";
                printUsage();
                return false;
            }
            std::string frequencyValue = argv[i + 1]; 
            try {
                std::size_t charactersRead = 0; // std::size_t is the standard unsigned type for sizes and counts
                options.frequency = std::stof(frequencyValue, &charactersRead); // see README: "why std::stof takes an address"
                // in 440abc stof sees the 440 and ignores abc
                if (charactersRead != frequencyValue.size()) {
                    std::cerr << "invalid frequency: " << frequencyValue << "\n"; 
                    printUsage();
                    return false;
                }
            }
            // std::exception is the standard library general error type
            catch (const std::exception&) {
                // runs only if stof threw (string can't be converted to a float)
                std::cerr << "invalid frequency: " << frequencyValue << "\n"; 
                printUsage();
                return false;
            }
            seenFrequency = true;
            i++;
        } 

        else if (argument == "-d" || argument == "--duration") {
            if (seenDuration) {
                std::cerr << argument << " given more than once" << "\n";
                printUsage();
                return false;
            }
            if (i+1 < argc && argv[i + 1][0] == '-' && std::isdigit(argv[i + 1][1])) {
                std::cerr << "duration must be between 0.5 and 60 seconds: " << argv[i + 1] << "\n";
                printUsage();
                return false;
            }
            if (!(i+1 < argc && argv[i + 1][0] != '-')) {
                std::cerr << "missing value for " << argument << "\n";
                printUsage();
                return false;

            }
            std::string durationValue = argv[i + 1];
            try {
                std::size_t charactersRead = 0;
                options.durationSeconds = std::stof(durationValue, &charactersRead);
                if (charactersRead != durationValue.size()) {
                    std::cerr << "invalid duration: " << durationValue << "\n"; 
                    printUsage();
                    return false;
                }
            }
            catch (const std::exception&) {
                std::cerr << "invalid duration: " << durationValue << "\n"; 
                printUsage();
                return false;
            }
            seenDuration = true;
            i++;
        }

        else if (argument == "-o" || argument == "--output") {
            if (seenOutput) {
                std::cerr << argument << " given more than once" << "\n";
                printUsage();
                return false;
            }
            if (!(i+1 < argc && argv[i + 1][0] != '-')) {
                std::cerr << "missing value for " << argument << "\n";
                printUsage();
                return false;
            }
            std::string outputValue = argv[i + 1]; 
            options.outputPath = outputValue;
            seenOutput = true;
            i++;
        }

        else if (argument == "-w" || argument == "--waveform") {
            if (seenWaveform) {
                std::cerr << argument << " given more than once" << "\n";
                printUsage();
                return false;
            }
            if (!(i+1 < argc && argv[i + 1][0] != '-')) {
                std::cerr << "missing value for " << argument << "\n";
                printUsage();
                return false;
            }
            std::string waveformName = argv[i + 1]; 
            std::string waveformLowered = waveformName;
            for (char& c : waveformLowered) {
                c = std::tolower(c);
            }
            if (waveformLowered == "sine") {
                options.waveform = Waveform::Sine;
                seenWaveform = true;
                i++;
            }
            else {
                std::cerr << "unknown waveform: " << waveformName << "\n";
                printUsage();
                return false;
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
            return false;
        }

    }
    return true;
}

// passing by plain reference, not const, as we append .wav in one of the branches
bool validateOptions(Options& options) {
    // range checks
    // std::stof returns "nan" as a special float that fails all comparisons. negating a check for valid fixes this.
    if (!(options.frequency >= 1 && options.frequency <= 20000)) {
        std::cerr << "frequency must be between 1 and 20000 Hz: " << options.frequency << "\n";
        printUsage();
        return false;
    }
    if (!(options.durationSeconds >= 0.5 && options.durationSeconds <= 60)) {
        std::cerr << "duration must be between 0.5 and 60 seconds: " << options.durationSeconds << "\n";
        printUsage();
        return false;
    }

    // output path rules
    if (options.outputPath.empty()) { 
        std::cerr << "output path cannot be empty" << "\n";
        printUsage();
        return false;
    }
    std::size_t lastDot = options.outputPath.rfind('.'); // finds the last "."
    std::size_t lastSlash = options.outputPath.rfind('/');
    // rfind returns a position, or npos (a huge number) if not found, never false, so compare against npos
    if (lastDot == std::string::npos || (lastSlash != std::string::npos && lastDot < lastSlash)) {
        options.outputPath += ".wav";
    }
    else {
        std::string pathExtension = options.outputPath.substr(lastDot);
        for (char& c : pathExtension) {
            c = std::tolower(c);
        }
        if (pathExtension != ".wav") {
            std::cerr << "output path must end in .wav or have no extension: " << options.outputPath << "\n";
            printUsage();
            return false;
        }
    }
    return true;
}
