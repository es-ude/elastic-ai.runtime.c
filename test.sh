#! /bin/bash

(
  cd build || exit

  cmake -DTARGET_GROUP=test ..

  make all

  #make test
  ctest -V #To see printf output from tests
)
