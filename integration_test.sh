#! /bin/bash

(
  cd build || exit

  cmake -DTARGET_GROUP=production ..

  make all

  ./bin/integration_test
)
