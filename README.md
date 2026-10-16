# MINISYNTH

## introduction

### goal

build a small synthesizer written in C++ using only the standard library, rendering multi-note, enveloped stereo audio from notes typed on the command line to a WAV file. real-time playback is an optional extension.

### checklist

- [x] **WAV file generator:** synthesize a sine wave tone and write it to a 16-bit mono WAV file using only the C++ standard library
- [x] **CLI tool:** split the code into functions and files, and set frequency, duration and output path via command-line arguments. add extensive error checks
- [ ] **oscillators:** generate sine, square, saw, triangle and noise waveforms using a phase accumulator
- [ ] **sequencer:** render a sequence of notes passed on the command line, with ADSR envelopes, stereo panning and mixing
- [ ] **real-time playback (optional):** play the synth live through the speakers using a callback-based audio API

### file structure

- the primary program file is `minisynth.cpp`
- `wav_file.cpp` and `wav_file.h` create the WAV file
- `tone_generator.cpp` and `tone_generator.h` generate the sound samples
- `options.cpp` and `options.h` read and check CLI inputted user options
- `/examples` contains code that explains relevant concepts

## commands

### run minisynth

- with defaults: `make run-minisynth`
- with arguments: `make run-minisynth ARGS="-f 440 -d 2.5 -o a440.wav"`

#### usage

`minisynth [-f|--frequency HZ] [-d|--duration SECONDS] [-o|--output PATH]`

#### options

all options are optional; any you leave out use their default. options can be given in any order.

- `-f`, `--frequency`: tone frequency in Hz, from 1 to 20000, decimals allowed | (default: 528)
- `-d`, `--duration`: length in seconds, from 0.5 to 60, decimals allowed | (default: 3)
- `-o`, `--output`: output file path, must either have no extension (`.wav` is added) or end in `.wav` (any capitalisation) | (default: `tone.wav`)

#### errors

- if an option or value isn't valid, minisynth prints an error and, for option errors, the usage line, and exits without writing a file
- see error reference at end of README for a full list of checked errors

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

**compiling and linking**

- each .cpp is compiled alone, so the compiler only knows of functions it's seen declarations for
- #including "headers" pastes in these declarations so the compiler can check even though the code is in other files
- the linker connects those calls to the definitions in the other object files

**source files**

- .cpp source files hold the definitions - the code behind the declarations
- every source .cpp with a header #includes it, so the compiler helps catch mismatches between definitions and declarations
- if an included header (e.g. `<cstring>`) is only used in a function body in the .cpp file and not in the .h, put it only in the .cpp

**header files**

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

### the blockAlign issue explained

- *why an odd number of bytes is a problem:* every sample is an `int16_t` which is 2 bytes. so the audio data is always 2 x number of samples, an even number. an odd number of bytes means “some whole samples, plus 1 extra byte”. 1 byte is half a sample, which doesn't exist, so that extra byte doesn't correspond to anything.
- *the header calculation:* in `wavfilegenerator`'s `createWavHeader` we worked out the number of bytes by `byteRate × duration`. `byteRate` is `2 * sampleRate` (see previous point), so essentially the number of bytes is worked out as `(2 * sampleRate) * duration`. with some decimal durations, this can lead to an odd number of bytes, e.g. `(2 * 44,100) × 1.00002 = 88,201.76`. we chop off the decimal and have an odd number: `88,201`. in `wavfilegenerator` we had a set duration of 3 seconds, so this was not a problem. but in `minisynth` the user can input decimal values for the duration. 
- *the generateTone calculation:* in `generateTone` the actual sample data is worked out differently: `sample rate x duration`. using the same calculation as above we get `44,100 × 1.00002 = 44,100.88`. _then_ we chop off the decimal, leaving us with `44,100`. how much actual space this takes up will always be this final number x 2, because as we know each sample takes up two bytes, and this will always be an even number. in our example, `2 x 44,100 = 88,200`
- *the problem:* now the header's audio data value of `88,201` doesn't match the actual audio data of `88,200`. writeFile now writes one byte more than the `samples` vector  created in `generateTone`actually holds, causing a memory bug.
- *the solution:* we work out the audio data in the `createWavHeader` the same way as we do in `generateTone`, by calculating the number of samples first: `uint32_t numSamples = header.sampleRate * durationSeconds;`then using this number in the subchunk2Size (audio data size) calculation, so `header.subchunk2Size = (header.byteRate) * durationSeconds` becomes `header.blockAlign * numSamples`. 

### object lifecycles and RAII (resource acquisition is initialization)

- `std::ofstream outputStream(...)` creates an *object*: an instance of a standard library class, which bundles data and functions (like `.write()`)
- a class can have a *constructor*, which runs automatically when the object is created (`std::ofstream`'s opens the file), and a *destructor*, which runs automatically when it's destroyed (`std::ofstream`'s closes the file)
- an object, like a variable, is destroyed when its scope ends: at the closing brace, or on any `return` out of the function (e.g. our `if (!outputStream)` checks)
- "destroyed" refers to the `outputStream` object (the variable), not the WAV file on disk, which stays
- so `outputStream` is always closed when `writeFile` ends, however it ends, and `outputStream.close()` isn't needed (it only exists to close a file earlier than the end of its scope)
- RAII = tying cleanup to an object's lifetime, so it happens automatically and can't be forgotten
- `std::vector` and `std::string` work the same way: their destructors free their memory, which is why we never free it ourselves

### variable data flow reference

- `durationSeconds`: we are using an existing variable, hence no type declaration
- `createWavHeader()` returns the *value* of its own "header" variable (which then disappears),
- then we store it in a NEW "header" variable here, for use in the following functions

### evaluation explanation

- example: `if (!(i+1 < argc && argv[i + 1][0] != '-'))`
- we evaluate the inner brackets first
- if `i+1 < argc` is `false`, it means there is no next argument, the value is missing
- the evaluation is "short circuited", so the rest of the expression doesn't evaluate, so the inner brackets in total evaluate to `false`
- the `!( .. )` _flips_ this to `true`, so the if block runs
- if `i+1 < argc` is `true`, we then check the right hand side of the expression
- if `argv[i + 1][0] != '-'` is `false`, it means there is a leading `-`, so it's an option not a value
- the inner brackets again evaluate to `false`, which is again flipped by the wrapping `!( .. )` to `true`, again running the if block
- if `argv[i + 1][0] != '-'` is `true`, then the inner brackets return true. this is flipped to `false` by the `!( .. )`, so we skip the if block

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
- could not write file: nofolder/440.wav

### tests

run each with `make run-minisynth ARGS="..."` (the first one with no ARGS). every error except "could not write file" also prints the usage line. no error writes a file.

**key test cases**

| checks | ARGS | expected output |
|---|---|---|
| defaults | *(none)* | `wrote tone.wav` |
| all options | `-f 440.5 -d 2.5 -o a440` | `wrote a440.wav` |
| missing value | `-f -d 2.5` | `missing value for -f` |
| unknown option | `--volume 5` | `unknown option: --volume` |
| value without option | `440` | `value without an option: 440` |
| duplicate | `-f 440 --frequency 880` | `--frequency given more than once` |
| invalid number | `-f 440abc` | `invalid frequency: 440abc` |
| negative value | `-d -5` | `duration must be between 0.5 and 60 seconds: -5` |
| bad extension | `-o tone.mp3` | `output path must end in .wav or have no extension: tone.mp3` |
| write failure | `-o nofolder/tone` | `could not write file: nofolder/tone.wav` |

**all test cases**

| area | ARGS | expected output |
|---|---|---|
| defaults | *(none)* | `wrote tone.wav` |
| all options | `-f 440 -d 2.5 -o a440` | `wrote a440.wav` |
| long forms | `--frequency 440 --duration 2 --output a440.wav` | `wrote a440.wav` |
| any order | `-o a440 -d 2 -f 440` | `wrote a440.wav` |
| structure | `-f` | `missing value for -f` |
| structure | `-f -d 2.5` | `missing value for -f` |
| structure | `--volume 5` | `unknown option: --volume` |
| structure | `440` | `value without an option: 440` |
| structure | `-f 440 300` | `value without an option: 300` |
| duplicates | `-f 440 -f 880` | `-f given more than once` |
| duplicates | `-f 440 --frequency 880` | `--frequency given more than once` |
| duplicates | `-d 1 -o a.wav -d 2` | `-d given more than once` |
| duplicates | `-o x.wav --output y.wav` | `--output given more than once` |
| frequency | `-f 440.5` | `wrote tone.wav` |
| frequency | `-f 1` | `wrote tone.wav` (lowest allowed) |
| frequency | `-f 20000` | `wrote tone.wav` (highest allowed) |
| frequency | `-f banana` | `invalid frequency: banana` |
| frequency | `-f 440abc` | `invalid frequency: 440abc` |
| frequency | `-f 1e999` | `invalid frequency: 1e999` (too big for a float) |
| frequency | `-f 0.5` | `frequency must be between 1 and 20000 Hz: 0.5` |
| frequency | `-f 25000` | `frequency must be between 1 and 20000 Hz: 25000` |
| frequency | `-f -5` | `frequency must be between 1 and 20000 Hz: -5` |
| frequency | `-f nan` | `frequency must be between 1 and 20000 Hz: nan` |
| frequency | `-f inf` | `frequency must be between 1 and 20000 Hz: inf` |
| duration | `-d 0.5` | `wrote tone.wav` (lowest allowed) |
| duration | `-d 60` | `wrote tone.wav` (highest allowed) |
| duration | `-d abc` | `invalid duration: abc` |
| duration | `-d 0.2` | `duration must be between 0.5 and 60 seconds: 0.2` |
| duration | `-d 90` | `duration must be between 0.5 and 60 seconds: 90` |
| duration | `-d -5` | `duration must be between 0.5 and 60 seconds: -5` |
| blockAlign | `-d 1.00002` | `wrote tone.wav`, and `ls -la tone.wav` shows 88244 bytes |
| output | `-o a440` | `wrote a440.wav` |
| output | `-o a440.wav` | `wrote a440.wav` |
| output | `-o a440.WAV` | `wrote a440.WAV` |
| output | `-o ./tone` | `wrote ./tone.wav` |
| output | `-o` | `missing value for -o` |
| output | `-o tone.mp3` | `output path must end in .wav or have no extension: tone.mp3` |
| output | `-o my.song` | `output path must end in .wav or have no extension: my.song` |
| output | `-o ""` (use `ARGS='-o ""'`) | `output path cannot be empty` |
| writing | `-o nofolder/tone` | `could not write file: nofolder/tone.wav` |

### known limitations

**endianness**

- little-endian machines store the lowest byte first. big-endian the largest. WAV format requires little-endian. most machines these days write using little-endian so that's ok, but for machines that write big-endian we need to write some extra functions. fortunately this does not affect any common machine today
- I'm parking this for the time being, until I can approach it with a better understanding of how bytes are stored, allocated and read, and how the conversion/re-write works.
