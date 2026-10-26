#pragma once

#include <string>

#include "waveform.h"

// Options holds our default "choices" (frequency, duration etc)
struct Options {
    float durationSeconds = 3; //seconds
    float frequency = 528; // Hz
    std::string outputPath = "tone.wav";
    Waveform waveform = Waveform::Sine;
};

bool parseOptions(int argc, char* argv[], Options& options);
bool validateOptions(Options& options);
