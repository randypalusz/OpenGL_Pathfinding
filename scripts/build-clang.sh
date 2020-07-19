#!/bin/bash

rm -rf build
mkdir -p build/clang
mkdir -p build/exe
cd build/clang
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ../..
make -j 16
ctest --output-on-failure

# go to build/, make symlinks to executables in build/exe/*.cc
cd ../
for file in $(find . -name "*.cc")
do
    ln -s ".$file" exe/
done