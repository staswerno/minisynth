#include <cmath>

#include "tone_generator.h"

// calculates the sample values of a sine tone
std::vector<int16_t> generateTone(float durationSeconds, uint32_t sampleRate, float frequency, int amplitude) {

    const double PI = 3.14159265358979323846;

    uint32_t numSamples = sampleRate * durationSeconds;
    std::vector<int16_t> samples(numSamples);

    double phaseIncrement = static_cast<double>(frequency) / static_cast<double>(sampleRate); // see float vs double
    double phasePosition = 0.0; 

    // refactored into a phase accumulator. see "phase accumulator explained" in README
    for (uint32_t i = 0; i < numSamples; i++) {
        samples[i] = static_cast<int16_t>(amplitude * sin(2 * PI * phasePosition)); // sine wave formula

        phasePosition += phaseIncrement;
        if (phasePosition >= 1) {
            phasePosition -= 1;
        }
    }

    return samples;
}
