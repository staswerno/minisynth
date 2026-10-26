#pragma once // solves: waveform.h is pasted twice into one .cpp, but C++ doesn't allow multiple definitions

enum class Waveform { Sine };

double waveformValue(Waveform waveform, double phasePosition);
