#! /bin/bash

(
  cd build || exit

  cmake -DTARGET_GROUP=test ..

  make all

  ctest -V # verbose shows print output from tests
)
