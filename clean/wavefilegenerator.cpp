#include <iostream>
#include <cstdint>
#include <cstring>
#include <vector>
#include <cmath>

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
    const double PI = 3.14159265358979323846;
    float durationSeconds = 3;

    WavHeader header;

    memcpy(header.riff, "RIFF", 4);
    memcpy(header.wave, "WAVE", 4);
    memcpy(header.fmt, "fmt ", 4);
    memcpy(header.data, "data", 4);
    header.subchunk1Size = 16;
    header.audioFormat = 1;
    header.numChannels = 1;
    header.sampleRate = 44100;
    header.bitsPerSample = 16;
    header.blockAlign = (header.bitsPerSample / 8) * header.numChannels;
    header.byteRate = header.blockAlign * header.sampleRate;
    header.subchunk2Size = header.byteRate * durationSeconds;
    header.chunkSize = 44 + header.subchunk2Size - 8;

    uint32_t numSamples = header.sampleRate * durationSeconds;
    std::vector<int16_t> samples(numSamples);
    int frequency = 528;
    int amplitude = 10000;

    for (uint32_t i = 0; i < numSamples; i++) {
        samples[i] = static_cast<int16_t>(amplitude * sin(2 * PI * frequency * i / header.sampleRate));
    }

    return 0;
}
