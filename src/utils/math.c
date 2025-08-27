#include "math.h"


unsigned long long udiv64(unsigned long long dividend, unsigned long long divisor) {
    unsigned long long quotient = 0;
    unsigned long long remainder = 0;

    for (int i = 63; i >= 0; i--) {
        remainder = (remainder << 1) | ((dividend >> i) & 1);
        if (remainder >= divisor) {
            remainder -= divisor;
            quotient |= (1ULL << i);
        }
    }
    return quotient;
}

unsigned long long umod64(unsigned long long dividend, unsigned long long divisor) {
    unsigned long long remainder = 0;
    for (int i = 63; i >= 0; i--) {
        remainder = (remainder << 1) | ((dividend >> i) & 1);
        if (remainder >= divisor) {
            remainder -= divisor;
        }
    }
    return remainder;
}
