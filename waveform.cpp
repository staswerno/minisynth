#include <cmath>

#include "waveform.h"

const double PI = 3.14159265358979323846;

double waveformValue(Waveform waveform, double phasePosition) {
    switch (waveform) {
        case Waveform::Sine:
            return sin(2 * PI * phasePosition); // sine wave formula
        case Waveform::Saw:
            return -1 + (1 - -1) * phasePosition; // saw wave formula
    }
    return 0.0;
}
