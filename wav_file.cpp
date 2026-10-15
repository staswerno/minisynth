#include <fstream>
#include <cstring>
#include <cstdint>
#include "wav_file.h"

// function definition leads with the return type - in this case the return type is WavHeader
// the compiler checks every return in the function against this promised return type
// this function creates a WavHeader variable based on previously defined type WavHeader, and fills in its fields
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
    uint32_t numSamples = header.sampleRate * durationSeconds;
    header.bitsPerSample = 16;
    header.blockAlign = (header.bitsPerSample / 8) * header.numChannels; // bytes per sample-frame
    header.byteRate = header.blockAlign * header.sampleRate; // bytes played per second
    header.subchunk2Size = header.blockAlign * numSamples; // size of the audio data
    header.chunkSize = sizeof(header) + header.subchunk2Size - 8; // total file size minus 8 bytes for "RIFF" and chunkSize fields

    return header;
}

// writes the WAV header and the audio data to a file
// we are passing the params as const references 
// const to make them read only, so the function doesn't make accidental changes (compiler enforced)
// & passes as reference so we don't unnecessarily copy the data (see pass-by-value-vs-reference.cpp)
bool writeFile(const WavHeader& header, const std::vector<int16_t>& samples, const std::string& outputPath) {
    std::ofstream outputStream(outputPath, std::ios::binary); // outputStream is our file, renamed for clarity when understanding RAII

    // std::ofstream doesn't check for opening/writing file fails, it just quietly fails, so we...
    // check for an open file fail, e.g. no permissions
    if (!outputStream) {
        return false;
    }

    outputStream.write(reinterpret_cast<const char*>(&header), sizeof(header));
    outputStream.write(reinterpret_cast<const char*>(samples.data()), header.subchunk2Size);
    // outputStream.close(); removed as file closes itself when writeFile ends due to RAII - see README for details on object lifecycles

    // check again for a write fail, e.g. full disk
    if (!outputStream) {
        return false;
    }

    return true;
}
