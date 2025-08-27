#ifndef CONVERSION_H
#define CONVERSION_H

#include <stdint.h>
#include "math.h"

int byteToKB(int bytes);
int byteToMB(int bytes);
int kBToMB(int kB);

int byteToKB_64u(uint64_t bytes);
int byteToMB_64u(uint64_t bytes);
int kBToMB_64u(uint64_t kB);

#endif