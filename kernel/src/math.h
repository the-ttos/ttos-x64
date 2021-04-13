#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

// Ceil macro
#define CEIL(x,y) (((x) + (y) - 1) / (y))

// 2D point
typedef struct {
    uint32_t x;
    uint32_t y;
} POINT;

// Power
uint64_t power_uint8(const uint8_t base, const uint8_t exponent) {
    uint64_t result = 1;
    for(uint8_t i = 0; i < exponent; i++)
        result *= base;
    return result;
}

// Ceil
uint64_t ceil(const uint32_t x, const uint32_t y) {
    uint64_t z = (uint64_t)x;
    return (z + y - 1) / y;
}