minisynth: minisynth.cpp wav_file.cpp wav_file.h tone_generator.cpp tone_generator.h
	clang++ minisynth.cpp wav_file.cpp tone_generator.cpp -std=c++17 -Wall -Wextra -o minisynth

# as "run" is an action, not a file, we declare it as a .PHONY target so it always runs
.PHONY: run

run: minisynth
	./minisynth