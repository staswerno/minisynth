// include library for input/output (essentially console printing)
// see test.cpp for more detailed understanding
#include <iostream>
// include fixed-width integer types
// wav files require fixed-width integer types for proper header formatting
#include <cstdint>
#include <cstring> // for memcpy
#include <vector> // for dynamic arrays (used for audio data)
#include <cmath> // for mathematical functions (used for generating audio samples)
#include <fstream> // for file input/output (used to write the wav file)

// forces no additional padding
#pragma pack(push, 1)
// struct is similar to grouping related data into an object in JS
// it bundles multiple named fields together into one custom type
// each field needs an explicit type declaration (eg uint32_t for 4-byte unsigned integer)
// you're defining a type (blueprint) not a value (a bit like a JS class)
struct WavHeader {
    // possible to assign here during declaration, e.g.
    // char riff[4] = {'R', 'I', 'F', 'F'};
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
// returns padding packing to default behaviour
#pragma pack(pop)

// every c++ program needs exactly one main function
int main() {

    // unsigned = no sign bit, so no negative values — just 0 and up
    // range depends on bit-width (e.g. uint8_t: 0-255, uint16_t: 0-65535)
    // output of uint32_t = 4 (size of int in bytes) - in this instance guaranteed, unlike "int"
    // note: byte-width tells you how much space something takes, but not what it means
    std::cout << "size of int: " << sizeof(int) << "\n"; // 4 - but not fixed
    std::cout << "size of uint32_t: " << sizeof(uint32_t) << "\n"; // 4
    std::cout << "size of uint16_t: " << sizeof(uint16_t) << "\n"; // 2
    std::cout << "size of uint8_t: " << sizeof(uint8_t) << "\n"; // 1

    // this actually outputs 44 "by luck of field ordering" (see README)
    // BUT sometimes compilers add padding bytes for performance reasons
    // (aligning data to certain memory boundaries makes CPUs faster at reading it)
    // we don't want this as the written wav would not match the expected header size
    // hence we use the #pragma pack (above) to force no additional padding
    std::cout << "size of WavHeader: " << sizeof(WavHeader) << "\n"; // 44 - but see above note

    // declare wav duration in seconds
    float durationSeconds = 3;

    const double PI = 3.14159265358979323846;

    //declares a variable named header of type WavHeader
    WavHeader header;

    // memcopy (memory copy) copies a block of memory from one location to another
    // means we don't need to set each character individually
    // e.g. header.riff[0] = 'R'; etc
    // memcpy(destination, source, howManyBytes)
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
    header.chunkSize = sizeof(header) + header.subchunk2Size - 8; // total file size - 8 bytes for "RIFF" and chunkSize fields

    // verify header values
    std::cout << "blockAlign: " << header.blockAlign << "\n";
    std::cout << "byteRate: " << header.byteRate << "\n";
    std::cout << "subchunk2Size: " << header.subchunk2Size << "\n";
    std::cout << "chunkSize: " << header.chunkSize << "\n";  

    // whenever a value scales with user input (duration, sample rate, file length, etc.)
    // default to a 32-bit type, unless you have a specific reason to go smaller
    uint32_t numSamples = header.sampleRate * durationSeconds; // total number of audio samples - 132,300
    std::vector<int16_t> samples(numSamples); // allocate 132,300 zero-initialized spaces for all audio samples, each a int16_t value

    // because freq and amp are internal calculations, not written to the file, they can be regular int types
    int frequency = 528; // Hz
    int amplitude = 10000; // peak amplitude, must stay within int16_t range (±32767)

    // explicitly type i, rather than "let" as in js
    for (uint32_t i = 0; i < numSamples; i++) {
        // static_cast converts double (decimal) value to samples[i]'s int16_t value
        // it basically chops off the decimal (not rounding)
        samples[i] = static_cast<int16_t>(amplitude * sin(2 * PI * frequency * i / header.sampleRate)); // sine wave formula, generates a value between -amplitude and +amplitude
        // print the first 20 samples for verification (and out of pure curiosity)
        if (i < 20) {
            std::cout << samples[i] << "\n";
        }
    }

    std::ofstream file("tone.wav", std::ios::binary); // binary writes raw bytes - no text-mode transformations
    // &header is WavHeader's memory address
    // char* relabels address as "pointing to raw bytes" (what write() requires)
    // think of char* a "byte type" (confusing, historical artifact)
    file.write(reinterpret_cast<const char*>(&header), sizeof(header)); // writes the header
    file.write(reinterpret_cast<const char*>(samples.data()), header.subchunk2Size); // writes the audio data
    file.close();

    // exit code - 0 means successful execution
    return 0;
}
