# MINISYNTH

## introduction

### goal

build a small synthesizer written in c++ using only the standard library, rendering multi-note, enveloped stereo audio from notes typed on the command line to a wav file. real-time playback is an optional extension.

### checklist

- [x] **wav file generator:** synthesize a sine wave tone and write it to a 16-bit mono wav file using only the c++ standard library
- [ ] **cli tool:** split the code into functions and files, and set frequency, duration and output path via command-line arguments
- [ ] **oscillators:** generate sine, square, saw, triangle and noise waveforms using a phase accumulator
- [ ] **sequencer:** render a sequence of notes passed on the command line, with ADSR envelopes, stereo panning and mixing
- [ ] **real-time playback (optional):** play the synth live through the speakers using a callback-based audio API

### file structure

the primary program file is `minisynth.cpp`

an uncommented version of the code is available in `./clean`

## commands

### run minisynth

`g++ minisynth.cpp -o minisynth && ./minisynth`

### run minisynth (clean)

`g++ clean/minisynth-clean.cpp -o clean/minisynth-clean && ./clean/minisynth-clean`

### check generated file size

`ls -la tone.wav`

## notes

