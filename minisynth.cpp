#include <iostream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>

#pragma pack(push, 1)
// remember, this is a type definition, not a value
struct WavHeader {
    char riff[4];
    uint32_t chunkSize;
    char wave[4];
    char fmt [4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char data[4];
    uint32_t subchunk2Size;
};
#pragma pack(pop)

// there is no hoisting in C++, code is read from top to bottom, so functions must be defined above the code that calls them
// function definition leads with the return type - in this case the return type is WavHeader
// the compiler checks every return in the function against this promised return type
// creates a WavHeader variable based on previously defined type WavHeader, and fills in its fields
WavHeader createWavHeader(float durationSeconds) {
    WavHeader header;

    memcpy(header.riff, "RIFF", 4);
    memcpy(header.wave, "WAVE", 4);
    memcpy(header.fmt, "fmt ", 4);
    memcpy(header.data, "data", 4);
    header.subchunk1Size = 16; // PCM header size
    header.audioFormat = 1; // PCM format
    header.numChannels = 1; // mono
    header.sampleRate = 44100;
    header.bitsPerSample = 16;
    header.blockAlign = (header.bitsPerSample / 8) * header.numChannels; // bytes per sample-frame
    header.byteRate = header.blockAlign * header.sampleRate; // bytes played per second
    header.subchunk2Size = header.byteRate * durationSeconds; // size of the audio data
    header.chunkSize = sizeof(header) + header.subchunk2Size - 8; // total file size minus 8 bytes for "RIFF" and chunkSize fields

    return header;
}

// calculates the sample values of a sine tone
std::vector<int16_t> generateTone(float durationSeconds, uint32_t sampleRate, int frequency, int amplitude) {
    const double PI = 3.14159265358979323846;

    uint32_t numSamples = sampleRate * durationSeconds;
    std::vector<int16_t> samples(numSamples);

    for (uint32_t i = 0; i < numSamples; i++) {
        samples[i] = static_cast<int16_t>(amplitude * sin(2 * PI * frequency * i / sampleRate)); // sine wave formula
    }

    return samples;
}

// writes the WAV header and the audio data to a file. returns nothing
void writeFile(WavHeader header, std::vector<int16_t> samples, std::string outputPath) {
    std::ofstream file(outputPath, std::ios::binary);
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    file.write(reinterpret_cast<const char*>(samples.data()), header.subchunk2Size);
    file.close();
}

// program starts here
// holds our "choices" (frequency, amplitude etc)
// runs the functions (passing values between them) necessary for the desired result of the program
int main() {
    float durationSeconds = 3; //seconds
    int frequency = 528; // Hz
    int amplitude = 10000; // peak amplitude, must stay within int16_t range (±32767)
    std::string outputPath = "tone.wav";

    // writing the type in a function call would turn it into a declaration, hence no type
    // durationSeconds: we are using an existing variable, hence no type declaration
    // createWavHeader() returns the *value* of its own "header" variable (which then disappears),
    // and we store it in a NEW "header" variable here, for use in the following functions
    WavHeader header = createWavHeader(durationSeconds);
    std::vector<int16_t> samples = generateTone(durationSeconds, header.sampleRate, frequency, amplitude);
    writeFile(header, samples, outputPath);

    return 0;
}
