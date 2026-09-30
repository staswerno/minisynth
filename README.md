# README

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
- - numeric fields use fixed-width int types matching that size (AudioFormat = 2 bytes -> uint16_t)
- - text markers (RIFF, WAVE, fmt , data) are 4-byte character sequences, not numbers
- - see below for full list of header fields
- 

### wav file header fields

- "RIFF" - 4 bytes - the literal ASCII characters R, I, F, F (a "magic marker" identifying this file type)
- ChunkSize - 4 bytes - total file size minus 8
- "WAVE" - 4 bytes - another literal marker
- "fmt " - 4 bytes - marker for the format section - NOTE: the empty space character
- Subchunk1Size - 4 bytes - size of the format section itself (always 16 for our purposes)
- AudioFormat - 2 bytes - 1 means "uncompressed PCM" (the simple kind we're generating)
- NumChannels - 2 bytes - 1 = mono, 2 = stereo
- SampleRate - 4 bytes - e.g. 44100 (samples per second)
- ByteRate - 4 bytes - bytes played per second
- BlockAlign - 2 bytes - bytes per sample-frame
- BitsPerSample - 2 bytes - e.g. 16
- "data" - 4 bytes - marker for the audio data section
- Subchunk2Size - 4 bytes - size of the actual audio data that follows