#include <cstdint>
#include <vector>
#include <string>
#include "wav_file.h"
#include "tone_generator.h"

// program starts here
// main holds our "choices" (frequency, amplitude etc)
// runs the functions (passing values between them) necessary for the desired result of the program
int main(int argc, char* argv[]) {
    float durationSeconds = 3; //seconds
    int frequency = 528; // Hz
    int amplitude = 10000; // peak amplitude, must stay within int16_t range (±32767)
    std::string outputPath = "tone.wav";

    // durationSeconds: we are using an existing variable, hence no type declaration
    // createWavHeader() returns the *value* of its own "header" variable (which then disappears),
    // and we store it in a NEW "header" variable here, for use in the following functions
    WavHeader header = createWavHeader(durationSeconds);
    std::vector<int16_t> samples = generateTone(durationSeconds, header.sampleRate, frequency, amplitude);
    writeFile(header, samples, outputPath);

    return 0;
}
