#! /bin/bash

cd build

cmake -DTARGET_GROUP=test ..

make all

make test

cd ..
