#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

// Ceiling function
#define CEILING(x,y) ((x + y - 1) / y)

// 2d point
typedef struct {
    unsigned x;
    unsigned y;
} point;

// Power function that uses 1 byte-sized base and exponent
uint64_t power_uint8(const uint8_t base, const uint8_t exponent) {
    uint64_t result = 1;
    for(uint8_t i = 0; i < exponent; i++)
        result *= base;
    return result;
}