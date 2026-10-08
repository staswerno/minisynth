minisynth: minisynth.cpp wav_file.cpp wav_file.h tone_generator.cpp tone_generator.h
	clang++ minisynth.cpp wav_file.cpp tone_generator.cpp -std=c++17 -Wall -Wextra -o minisynth

