#! /bin/bash

cmake -B cmake-build-debug -G Ninja .

cmake --build cmake-build-debug -j 4

ctest --test-dir cmake-build-debug/test --output-on-failure
