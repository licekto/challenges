#!/bin/bash

mkdir -p Build/Debug Build/Release

cd Build/Debug

cmake -DCMAKE_BUILD_TYPE=Debug ../..
make all -j`nproc`
./advent-of-code-2022 --test

cd ../Release

cmake -DCMAKE_BUILD_TYPE=Release ../..
make all -j`nproc`
./advent-of-code-2022 --test

cd ../..
rm -rf Build
