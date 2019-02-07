#!/bin/sh
# test files
gcc -shared -fPIC -g -std=c++14 -c -o asm32.o     asm32.cc     -I.
gcc -shared -fPIC -g -std=c++14 -c -o stub32.o    stub32.cc    -I.
gcc -shared -fPIC -g -std=c++14 -c -o binreader.o binreader.cc -I.
gcc -shared -fPIC -g -std=c++14 -c -o binwriter.o binwriter.cc -I.
gcc -shared -fPIC -g            -c -o vmexec.o    vmexec.c     -I.

# test lib
gcc -z defs -shared -fPIC -g -std=c++14 -o libstub32.so \
	asm32.o     \
	vmexec.o    \
	binreader.o \
	binwriter.o \
	\
	-L. -lasmjit -lasmtk -lstdc++

# test program
gcc -g -std=c++14 -o test stub32.o -L. -l stub32 -lasmjit -lasmtk -lstdc++
