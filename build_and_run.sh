#! /bin/bash

cd build

cmake -DTARGET_GROUP=production .

make all

./bin/release

cd ..