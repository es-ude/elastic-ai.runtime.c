# elastic-ai.runtime.c

## Prerequisites

* cmake >= 3.2.1
* gcc or clang

The project does out-of-source builds in the `build` directory. It does not need to be created manually if you use
the `test.sh` shell script.

## Setup

First, you need to load CMake once (CLion does that for you, under CMake, you can change the generator):

```bash
cmake -B cmake-build-debug -G Ninja .
```

The flag `-G Ninja` tells CMake to use Ninja as the build tool.
If this flag is not passed, CMake will use the default build tool on your machine (mostly Makefiles).

## Unit Tests

Unit Tests use [ThrowTheSwitch/Unity](https://github.com/ThrowTheSwitch/Unity) as a testing framework, which will be
fetched automatically by CMake.

## Shell Scripts

Build and run tests: `test.sh`
