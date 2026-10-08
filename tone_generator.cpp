#include <cmath>
#include "tone_generator.h"

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
