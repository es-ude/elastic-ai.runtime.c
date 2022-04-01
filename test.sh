#! /bin/bash

if ! [[ -d "build" ]]; then
    mkdir build
fi

(
  cd build || exit

  cmake -DTARGET_GROUP=test ..

  make all

  ctest -V # verbose shows print output from tests
)
