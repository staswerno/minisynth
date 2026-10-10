# a target is normally a file: its recipe only runs if the file is missing or older than a prerequisite (see README)
# .PHONY targets are actions, not files, so their recipes always run
.PHONY: run-minisynth run-pass-by-value-vs-reference run-command-line-arguments clean

minisynth: minisynth.cpp wav_file.cpp wav_file.h tone_generator.cpp tone_generator.h
	clang++ minisynth.cpp wav_file.cpp tone_generator.cpp -std=c++17 -Wall -Wextra -o minisynth

# changes from wavefilegenerator:
# changed `g++` to `clang++` for clarity - on macOS `g++` forwards to Apple clang
# `-std=c++17` tells the compiler which language version to use when reading the code
# the `c++17` standard is supported by a wide range of compilers and has required features for project
# `-Wall` and `-Wextra` enable both common and extra warnings

# a target can be another target's prerequisite: `make run-minisynth` builds `minisynth` first if needed
run-minisynth: minisynth
	./minisynth

examples/pass-by-value-vs-reference: examples/pass-by-value-vs-reference.cpp
	clang++ examples/pass-by-value-vs-reference.cpp -std=c++17 -Wall -Wextra -o examples/pass-by-value-vs-reference

run-pass-by-value-vs-reference: examples/pass-by-value-vs-reference
	./examples/pass-by-value-vs-reference

examples/command-line-arguments: examples/command-line-arguments.cpp
	clang++ examples/command-line-arguments.cpp -std=c++17 -Wall -Wextra -o examples/command-line-arguments

run-command-line-arguments: examples/command-line-arguments
	./examples/command-line-arguments

clean:
	rm -f minisynth examples/pass-by-value-vs-reference examples/command-line-arguments *.wav

