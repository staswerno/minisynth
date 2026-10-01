// include library for input/output (essentially console printing)
// see test.cpp for more detailed understanding
#include <iostream>
// include fixed-width integer types
// wav files require fixed-width integer types for proper header formatting
#include <cstdint>
#include <cstring> // for memcpy

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
    // uint = unsigned integer to represent only non-negative values (0-255 ∴ 256)
    // output of uint32_t = 4 (size of int in bytes) - in this instance guaranteed, unlike "int"
    // note: byte-width tells you how much space something takes, but not what it means
    std::cout << "size of int: " << sizeof(int) << "\n"; // 4
    std::cout << "size of uint32_t: " << sizeof(uint32_t) << "\n"; // 4
    std::cout << "size of uint16_t: " << sizeof(uint16_t) << "\n"; // 2
    std::cout << "size of uint8_t: " << sizeof(uint8_t) << "\n"; // 1

    // this actually outputs 44 "by luck of field ordering" (see README)
    // BUT sometimes compilers add padding bytes for performance reasons
    // (aligning data to certain memory boundaries makes CPUs faster at reading it)
    // we don't want this as the written wav would not match the expected header size
    // hence we use #pragma pack (above) to force no additional padding
    std::cout << "size of WavHeader: " << sizeof(WavHeader) << "\n"; // 44 - but see above note

    // declare wav duration in seconds
    float durationSeconds = 3;

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
    header.chunkSize = 44 + header.subchunk2Size - 8; // total file size - 8 bytes for "RIFF" and chunkSize fields

    // verify header values
    std::cout << "header.blockAlign: " << header.blockAlign << "\n";
    std::cout << "header.byteRate: " << header.byteRate << "\n";
    std::cout << "header.subchunk2Size: " << header.subchunk2Size << "\n";
    std::cout << "header.chunkSize: " << header.chunkSize << "\n";  

    
    // exit code - 0 means successful execution
    return 0;
}

