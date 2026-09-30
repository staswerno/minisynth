#include <iostream>
#include <cstdint>
#include <cstring>

#pragma pack(push, 1)
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

int main() {
    std::cout << sizeof(int) << "\n";
    std::cout << sizeof(uint32_t) << "\n";
    std::cout << sizeof(uint16_t) << "\n";
    std::cout << sizeof(uint8_t) << "\n";
    std::cout << sizeof(WavHeader) << "\n";
    return 0;

    WavHeader header;

    memcpy(header.riff, "RIFF", 4);
    memcpy(header.wave, "WAVE", 4);
    memcpy(header.fmt, "fmt ", 4);
    header.audioFormat = 1; // PCM format
    header.numChannels = 1; // mono
    header.sampleRate = 44100;
    header.bitsPerSample = 16;

}

