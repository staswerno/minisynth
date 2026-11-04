#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'

passedCount=0
failedCount=0

check() {
    result=$(./minisynth $1 2>&1) # run minisynth with the given arguments, capture both stdout and stderr, store in "result"
    argument="$1"
    if [[ $1 == "" ]]; then
        argument="no argument"
    fi
    if [[ "$result" == *"$2"* ]]; then
        passedCount=$((passedCount + 1))
        echo -e "${GREEN}PASS: $argument${RESET}"
    else
        failedCount=$((failedCount + 1))
        echo -e "${RED}FAIL: $argument${RESET}"
        echo -e "  ${RED}expected: $2${RESET}"
        echo -e "${RED}got: $result${RESET}" | sed 's/^/  /' #  sed adds two spaces to the start of every line of the result, including the usage line
    fi
}

# defaults and combinations
check "" "wrote tone.wav"
check "-f 440 -d 2.5 -o a440" "wrote a440.wav"
check "--frequency 440 --duration 2 --output a440.wav" "wrote a440.wav"
check "-o a440 -d 2 -f 440" "wrote a440.wav"
check "-o a440 -w sine" "wrote a440.wav"
check "-w sine -o a440" "wrote a440.wav"

# structure
check "-f" "missing value for -f"
check "-f -d 2.5" "missing value for -f"
check "--volume 5" "unknown option: --volume"
check "440" "value without an option: 440"
check "-f 440 300" "value without an option: 300"
check "-w" "missing value for -w"

# duplicates
check "-f 440 -f 880" "-f given more than once"
check "-f 440 --frequency 880" "--frequency given more than once"
check "-d 1 -o a.wav -d 2" "-d given more than once"
check "-o x.wav --output y.wav" "--output given more than once"
check "-w sine --waveform sine" "--waveform given more than once"

# frequency
check "-f 440.5" "wrote tone.wav"
check "-f 1" "wrote tone.wav"
check "-f 20000" "wrote tone.wav"
check "-f banana" "invalid frequency: banana"
check "-f 440abc" "invalid frequency: 440abc"
check "-f 1e999" "invalid frequency: 1e999"
check "-f 0.5" "frequency must be between 1 and 20000 Hz: 0.5"
check "-f 25000" "frequency must be between 1 and 20000 Hz: 25000"
check "-f -5" "frequency must be between 1 and 20000 Hz: -5"
check "-f nan" "frequency must be between 1 and 20000 Hz: nan"
check "-f inf" "frequency must be between 1 and 20000 Hz: inf"

# duration
check "-d 0.5" "wrote tone.wav"
check "-d 60" "wrote tone.wav"
check "-d abc" "invalid duration: abc"
check "-d 0.2" "duration must be between 0.5 and 60 seconds: 0.2"
check "-d 90" "duration must be between 0.5 and 60 seconds: 90"
check "-d -5" "duration must be between 0.5 and 60 seconds: -5"

# blockAlign: file size is checked by hand, see "special test cases" in README
check "-d 1.00002" "wrote tone.wav"

# output
check "-o a440" "wrote a440.wav"
check "-o a440.wav" "wrote a440.wav"
check "-o a440.WAV" "wrote a440.WAV"
check "-o ./tone" "wrote ./tone.wav"
check "-o" "missing value for -o"
check "-o tone.mp3" "output path must end in .wav or have no extension: tone.mp3"
check "-o my.song" "output path must end in .wav or have no extension: my.song"
# -o "" (empty path) is tested by hand: bash can't pass an empty argument through an unquoted $1
# run: make run-minisynth ARGS='-o ""'   expected: output path cannot be empty

# writing
check "-o nofolder/tone" "could not write file: nofolder/tone.wav"

# waveform
check "-w sine" "wrote tone.wav"
check "-w saw" "wrote tone.wav"
check "-w square" "wrote tone.wav"
check "-w triangle" "wrote tone.wav"
check "-w noise" "wrote tone.wav"
check "--waveform saw" "wrote tone.wav"
check "-w SQUARE" "wrote tone.wav"
check "-w banjo" "unknown waveform: banjo"
check "-w Banjo" "unknown waveform: Banjo"

echo -e "${GREEN}$passedCount PASSED${RESET}"

if [[ $failedCount -gt 0 ]]; then
    echo -e "${RED}$failedCount FAILED${RESET}"
    exit 1
else
    echo -e "${GREEN}0 FAILED${RESET}"
fi
