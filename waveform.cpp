#include <cmath>

#include "waveform.h"

const double PI = 3.14159265358979323846;

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
    }
    return 0.0;
}
