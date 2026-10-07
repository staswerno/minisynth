# MINISYNTH

## introduction

### goal

build a small synthesizer written in C++ using only the standard library, rendering multi-note, enveloped stereo audio from notes typed on the command line to a WAV file. real-time playback is an optional extension.

### checklist

- [x] **WAV file generator:** synthesize a sine wave tone and write it to a 16-bit mono WAV file using only the C++ standard library
- [ ] **CLI tool:** split the code into functions and files, and set frequency, duration and output path via command-line arguments
- [ ] **oscillators:** generate sine, square, saw, triangle and noise waveforms using a phase accumulator
- [ ] **sequencer:** render a sequence of notes passed on the command line, with ADSR envelopes, stereo panning and mixing
- [ ] **real-time playback (optional):** play the synth live through the speakers using a callback-based audio API

### file structure

the primary program file is `minisynth.cpp`

an uncommented version of the code is available in `./clean`

## commands

### run minisynth

`clang++ minisynth.cpp -std=c++17 -Wall -Wextra -o minisynth && ./minisynth`

- changed `g++` to `clang++` for clarity - on macOS `g++` forwards to Apple clang
- `-std=c++17` tells the compiler which language version to use when reading the code
- the `c++17` standard is supported by a wide range of compilers and has required features for project
- `-Wall` and `-Wextra` enable both common and extra warnings

### run minisynth (clean)

`clang++ clean/minisynth-clean.cpp -std=c++17 -Wall -Wextra -o clean/minisynth-clean && ./clean/minisynth-clean`

### check generated file size

`ls -la tone.wav`

### run pass-by-value experiment

`clang++ pass-by-value-vs-reference.cpp -std=c++17 -Wall -Wextra -o pass-by-value-vs-reference && ./pass-by-value-vs-reference`

## notes

