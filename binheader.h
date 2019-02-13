#ifndef BINHEADER_H
#define BINHEADER_H
#pragma once

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cctype>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <type_traits>
#include <map>
#include <vector>
#include <utility>      // std::pair

#include "./asmjit/asmjit.h"
#include "./asmtk/asmtk.h"
using namespace std;
using namespace asmjit;
using namespace asmtk;

#include "binreader.h"
#include "binwriter.h"

#include "emitter.h"

#define EXPORT __attribute__((visibility("default")))
#define IMPORT

extern "C" void vmExec(uint8_t *img, size_t len);

struct Externals {
    void *funcaddr;
} externals[2] = {
    { &puts },      // +0
    { &printf }     // +4
};

typedef void (*func)(char *);   // signature

# define EXTERN_(x)      externals[x].funcaddr;
# define EXTERN_PUTS     EXTERN_(0)

#endif
