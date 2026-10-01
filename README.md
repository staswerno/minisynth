# README

the primary program file is `wavefilegenerator.cpp`

an uncommented version of the code is available in `./clean`

## commands

### run test

`g++ test.cpp -o test && ./test` <br>
compiles `test.cpp`, and names the output binary "test" <br>
the `-o` flag means "output" <br>
`&&` will run `./test` if the preceeding command succeeds

### run wave file generator

`g++ wavefilegenerator.cpp -o wavefilegenerator && ./wavefilegenerator`

## notes

### wav files

- wav files start with a 44-byte header broken into 3 sections
- each header field has a fixed byte-width
    - numeric fields use fixed-width int types matching that size (AudioFormat = 2 bytes -> `uint16_t`)
    - text markers (RIFF, WAVE, fmt , data) are 4-byte character sequences, not numbers
    - see below for full list of header fields


### wav file header fields

| Field | Size | Type | Description |
|---|---|---|---|
| "RIFF" | 4 bytes | char[4] | the literal ASCII characters R, I, F, F (a "magic marker" identifying this file type) |
| ChunkSize | 4 bytes | uint32_t | total file size minus 8 |
| "WAVE" | 4 bytes | char[4] | another literal marker |
| "fmt " | 4 bytes | char[4] | marker for the format section — NOTE: the empty space character |
| Subchunk1Size | 4 bytes | uint32_t | size of the format section itself (always 16 for our purposes) |
| AudioFormat | 2 bytes | uint16_t | 1 means "uncompressed PCM" (the simple kind we're generating) |
| NumChannels | 2 bytes | uint16_t | 1 = mono, 2 = stereo |
| SampleRate | 4 bytes | uint32_t | e.g. 44100 (samples per second) |
| ByteRate | 4 bytes | uint32_t | bytes played per second |
| BlockAlign | 2 bytes | uint16_t | bytes per sample-frame |
| BitsPerSample | 2 bytes | uint16_t | e.g. 16 |
| "data" | 4 bytes | char[4] | marker for the audio data section |
| Subchunk2Size | 4 bytes | uint32_t | size of the actual audio data |

### notes on WavHeader byte size output

"padding rules generally require multi-byte numeric fields to sit at memory addresses divisible by their own size"

- a `uint32_t` wants to start at an offset divisible by 4
- a `uint16_t` at an offset divisible by 2

in `wavefilegenerator.cpp` the field order happens to naturally satisfy this without gaps - each field lands exactly where the next one needs it to

in a different order, for example flipping a `uint16_t` and `uint32_t` the compiler would (in some cases) have silently inserted padding bytes to keep the `uint32_t` aligned, and sizeof would come back larger than expected

in `test.cpp`, `uint32_t` needs to start with an offset of 4, but `uint8_t` only take up one byte, so the compiler adds in 3 bytes