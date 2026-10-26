#pragma once

#include <vector>
#include <cstdint>

#include "waveform.h"

std::vector<int16_t> generateTone(float durationSeconds, uint32_t sampleRate, float frequency, int amplitude, Waveform waveform);
