#include <cstdint>
#include <vector>
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

WavHeader createWavHeader(float durationSeconds);
void writeFile(const WavHeader& header, const std::vector<int16_t>& samples, const std::string& outputPath);
