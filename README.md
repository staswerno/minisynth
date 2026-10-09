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

- the primary program file is `minisynth.cpp`
- `wav_file.cpp` and `wav_file.h` create the WAV file
- `tone_generator.cpp` and `tone_generator.h` generate the sound samples
- `/examples` contains code that explains relevant concepts

## commands

### run minisynth

`make run-minisynth`

### check generated file size

`ls -la tone.wav`

### run pass-by-value-vs-reference experiment

`make run-pass-by-value-vs-reference`

### clean output files

`make clean`

## notes

### code structure

- there is no hoisting in C++, code is read from top to bottom, so functions must be defined above the code that calls them
- declarations pasted in at the top of a file make functions known before any code that calls them (see below)

### source and header files

- each .cpp is compiled alone, so the compiler only knows of functions it's seen declarations for
- #including "headers" pastes in these declarations so the compiler can check even though the code is in other files
- the linker connects those calls to the definitions in the other object files

- .cpp source files hold the definitions - the code behind the declarations
- every source .cpp with a header #includes it, so the compiler helps catch mismatches between definitions and declarations
- if an included header (e.g. `<cstring>`) is only used in a function body in the .cpp file and not in the .h, put it only in the .cpp

- a .h header file is the export list of its header/source pair: types plus declarations, with no function bodies
- `minisynth.cpp` has no header file, because nothing calls main: a header is only needed for things other files use
- it’s a single source of truth: one copy of the WavHeader struct, so other files can’t disagree about its layout
- include everything that the function declarations and types use
- even if a file gets its includes through the included headers, it's good practice to add every include that you need in case there are changes to the other file

### const variable declarations

- const means the value can't be changed
- any value that doesn't change after the variable is created could be a const
- eg frequency, amplitude but not samples, i in `generateTone`
- whether to mark everything possible as const is a style choice

### pass by value and references

- C++ copies arguments by default: the function gets its own copy (pass by value)
- `&` after a type makes a parameter a reference: no copy, the function works on the caller's original
- `const &` makes the reference read-only, so you get no copy and no accidental changes
- small types (`int`, `float`) are fine to pass by value; bigger things the function only reads (vectors, strings, structs) are usually passed by `const &`
- see `examples/pass-by-value-vs-reference.cpp`

### Makefile

```make
greeting: hello.cpp
	clang++ hello.cpp -o greeting
```

"to make `greeting`, you need `hello.cpp`, and here's the command"

- **target** (`greeting`): usually the file the recipe creates; for a `.PHONY` target, just a name for an action
- **prerequisites** (`hello.cpp`): the files the target depends on; the recipe only runs if the target is missing or a prerequisite is newer than it
- **recipe** (the indented line): the command that creates the target; must be indented with a tab, not spaces
- `make` builds the first target in the file; `make <target>` builds a specific one