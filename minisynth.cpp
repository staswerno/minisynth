#include <iostream>
#include <cstdint>
#include <vector>

#include "wav_file.h"
#include "tone_generator.h"
#include "options.h"

// program starts here
// runs the functions (passing values between them) necessary for the desired result of the program 
int main(int argc, char* argv[]) {
    Options options;
    int amplitude = 10000; // peak amplitude, must stay within int16_t range (±32767)

    // the [] of argv[] is part of the type definition so we don't pass it in
    if (!parseOptions(argc, argv, options)) {
        return 1; // exit the program with an error code. nothing more runs, so nothing is written
    }
    if (!validateOptions(options)) {
        return 1;
    }

    // durationSeconds: see README for example data flow
    WavHeader header = createWavHeader(options.durationSeconds);
    std::vector<int16_t> samples = generateTone(options.durationSeconds, header.sampleRate, options.frequency, amplitude, options.waveform);
    bool fileWritten = writeFile(header, samples, options.outputPath);

    // check output file
    if (!fileWritten) {
        std::cerr << "could not write file: " << options.outputPath << "\n";
        return 1;
    }

    std::cout << "wrote " << options.outputPath << "\n";
    return 0;
}
