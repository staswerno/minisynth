#!/bin/bash

passedCount=0
failedCount=0

check() {
    result=$(./minisynth $1 2>&1)
    echo "$result"
}

check ""