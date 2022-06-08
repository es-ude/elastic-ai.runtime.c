#! /bin/bash

cmake --build cmake-build-debug -j 4

ctest --test-dir cmake-build-debug/test -V
