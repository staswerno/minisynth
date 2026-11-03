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

check "" "wrote tone.wav"
check "-f banana" "invalid frequency: banana"

echo -e "${GREEN}$passedCount PASSED${RESET}"

if [[ $failedCount -gt 0 ]]; then
    echo -e "${RED}$failedCount FAILED${RESET}"
    exit 1
else
    echo -e "${GREEN}0 FAILED${RESET}"
fi
