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

`make run-minisynth ARGS="-f 440 -d 2.5 -o a440.wav"`

#### usage

`minisynth [-f|--frequency HZ] [-d|--duration SECONDS] [-o|--output PATH]`

#### options

all options are optional; any you leave out use their default. options can be given in any order.

- `-f`, `--frequency`: tone frequency in Hz, from 1 to 20000 (_implementation in progress_), decimals allowed (default: 528)
- `-d`, `--duration`: length in seconds, from 0.5 to 60 (_implementation in progress_), decimals allowed (default: 3)
- `-o`, `--output`: output file path, must either have no extension or end in `.wav` (any capitalisation) (_implementation in progress_) (default: `tone.wav`)

#### errors (_implementation in progress_)

if an option or value isn't valid, minisynth prints an error and the usage line, and exits without writing a file.

### check generated file size

`ls -la tone.wav`

### run pass-by-value-vs-reference experiment

`make run-pass-by-value-vs-reference`

### run command-line-arguments

`make examples/command-line-arguments && ./examples/command-line-arguments hello 440 2.5`

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

### why std::stof takes an address

```
std::size_t charactersRead = 0;
frequency = std::stof(frequencyValue, &charactersRead);
```

a function only has one return type, already used for the number (stored in `frequency`) so the count needs to come back another way. using `&charactersRead` writes directly to `charactersRead` rather than copying it (an "output parameter" - function writes to, rather than reads from, it). we use an address rather than a reference because it can also be "nothing" (a null pointer), making the parameter optional. 

### error messages reference

- unknown option: --volume
- missing value for --frequency
- value without an option: 440
- --frequency given more than once
- invalid frequency: banana
- frequency must be between 1 and 20000 Hz: 25000
- invalid duration: abc
- duration must be between 0.5 and 60 seconds: 90
- output path cannot be empty
- output path must end in .wav or have no extension: tone.mp3
