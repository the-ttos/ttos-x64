#ifndef LIMITS_H
#define LIMITS_H
#include <limits.h>
#endif

#ifndef MATH_H
#define MATH_H
#include "math.h"
#endif

// 1 trit = 2 bits
#define TRIT_BIT CEILING(3, 2)

// 1 tryte = 9 trits
#define TRYTE_TRIT 9

// 1 word = 27 trits
#define WORD_TRIT 27

// 1 byte = 4 trits
#define BYTE_TRIT (CHAR_BIT / TRIT_BIT)

// 1 tryte = 3 bytes
#define TRYTE_BYTE CEILING(TRYTE_TRIT, BYTE_TRIT)

// 1 word = 9 bytes
#define WORD_BYTE CEILING(WORD_TRIT, BYTE_TRIT)

// 1 heptavintimal character = 3 trytes
#define HEPTA_TRYTE (WORD_TRIT / TRYTE_TRIT)

// Tryte macro declaration
#define __tryte(name) uint8_t name[TRYTE_BYTE]

// Word macro declaration
#define __word(name) uint8_t name[WORD_BYTE]

// Byte a trit is in
#define __trit_byte(t) (t * TRIT_BIT / CHAR_BIT)

// Negate trit
// xy
// 00 | 2
// 01 | 1
// 11 | X
// 10 | 0
unsigned char *__negate(const __tryte(t)) {
    static __tryte(u);
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t mask = 0b10 << (BYTE_TRIT - 1 - i % BYTE_TRIT) * 2;
        const uint8_t y = (t[__trit_byte(i)] & (mask >> 1)) << 1;
        u[__trit_byte(i)] &= ~mask;
        u[__trit_byte(i)] |= (t[__trit_byte(i)] ^ ~y) & mask;
    }
    return u;
}
