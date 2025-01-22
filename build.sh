#!/bin/bash

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

make clean
make all
#$HOME/opt/cross/bin/$TARGET-gcc --version
