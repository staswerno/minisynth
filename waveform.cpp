#include <cmath>
#include <random>

#include "waveform.h"

const double PI = 3.14159265358979323846;

std::mt19937 generator;                                    // std libs general purpose generator. makes raw random numbers...
std::uniform_real_distribution<double> range(-1.0, 1.0);   // ...shapes them into 0 to 10, type double

double waveformValue(Waveform waveform, double phasePosition) {
    switch (waveform) {
        case Waveform::Sine:
            return sin(2 * PI * phasePosition); // sine wave formula
        case Waveform::Saw:
            return -1 + (1 - -1) * phasePosition; // saw wave formula
        case Waveform::Square:
            if (phasePosition < 0.5) {
                return 1.0;
            }
            return -1.0; // square wave formula
        case Waveform::Triangle:
            if (phasePosition < 0.5) {
                return -1 + (1 - -1) * (phasePosition * 2);
            }
            return 1 + (-1 - 1) * ((phasePosition - 0.5) * 2); // triangle wave formula
        case Waveform::Noise:
            return range(generator);
    }
    return 0.0;
}
