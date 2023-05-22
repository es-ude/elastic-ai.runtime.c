# elastic-ai.runtime.c

This repository encapsulates the information about the elastic-ai.runtime protocol in simple C 
to be included as a library for various projects that want to use the protocol.

For easy integration the CMake library ```elastic-ai.runtime.c``` is provided that can be used when this repository is included as a submodule.
For the library to work a MQTT broker implementation must be created that implements all the functions defined in [CommunicationEndpoint.h](src/protocol/CommunicationEndpoint.h), as shown in the [example implementation](src/exampleLocalBroker/ExampleLocalBroker.c).

## Prerequisites

* cmake >= 3.2.1
* gcc or clang
* (Ninja)

## Setup

First, CMake needs to be loaded once (When using CLion, the provided CMake configuration can be used):
```bash
cmake -B cmake-build-test -G Ninja .
```

The flag `-G Ninja` tells CMake to use Ninja as the build tool.
If this flag is not passed, CMake will use the default build tool on your machine (mostly Makefiles).

Secondly the git submodules need to be initialized:
```bash
git submodule update --init --recursive
```

## Unit Tests

The Unit Tests use [ThrowTheSwitch/Unity](https://github.com/ThrowTheSwitch/Unity) as a testing framework.

First the tests need to be built:
```bash
cmake --build cmake-build-test 
```

After that they can be executed:
```bash
ctest --test-dir cmake-build-test 
```

(When using CLion the provided run configuration can be used instead of both commands.)
