#include <cmath>
#include <random>

#include "waveform.h"

const double PI = 3.14159265358979323846;

std::mt19937 generator; // std libs general purpose generator. makes raw random numbers...
std::uniform_real_distribution<double> range(-1.0, 1.0); // ...shapes them into -1 to 1, type double

double waveformValue(Waveform waveform, double phasePosition) {
    // see README for notes on waveform formulas
    switch (waveform) {
        case Waveform::Sine:
            return sin(2 * PI * phasePosition);
        case Waveform::Saw:
            return -1 + (1 - -1) * phasePosition;
        case Waveform::Square:
            if (phasePosition < 0.5) {
                return 1.0;
            }
            return -1.0;
        case Waveform::Triangle:
            if (phasePosition < 0.5) {
                return -1 + (1 - -1) * (phasePosition * 2);
            }
            return 1 + (-1 - 1) * ((phasePosition - 0.5) * 2);
        case Waveform::Noise:
            return range(generator);
    }
    return 0.0;
}
