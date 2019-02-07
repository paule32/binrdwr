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

constexpr int ASM32_MAGIC   = 0x19790208;
constexpr int ASM32_VERSION = 0x00000132;

#endif
