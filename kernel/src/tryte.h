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
#define HEPTA_TRYTE (27 / TRYTE_TRIT)

// Tryte macro declaration
#define __tryte(name) uint8_t name[TRYTE_BYTE]

// Word macro declaration
#define __word(name) uint8_t name[WORD_BYTE]

// Byte a trit is in
#define __byte_of_trit(t) ((t) * TRIT_BIT / CHAR_BIT)

// Offset of 0b11 inside a byte
#define __trit_offset(i) ((BYTE_TRIT - 1 - (i) % BYTE_TRIT) * TRIT_BIT);

// NOT gate (KARNAUGH)
// Negate trit
// xy | z
// 00 | 2
// 01 | 1
// 11 | X
// 10 | 0
//
// This implementation uses the multibase k-map equation:
// z = (~x & ~y) * 2 + (~x & y)
unsigned char *__not_k(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t notX = (~t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        const uint8_t notY = ~t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= (notX & notY) * 2 + (notX & y);
    }
    return u;
}

// NOT gate
// Negate trit
// xy | z
// 00 | 2
// 01 | 1
// 11 | X
// 10 | 0
//
// This implementation uses the following equation:
// x' = x ^ ~y; y' = y
unsigned char *__not(const __tryte(t)) {
    static __tryte(u);
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t notY = ~t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b10 << offset);
        u[__byte_of_trit(i)] |= x ^ notY << 1;
    }
    return u;
}

// INC gate (KARNAUGH)
// Increment trit (modulo)
// xy | z
// 00 | 1
// 01 | 2
// 11 | X
// 10 | 0
//
// This implementation uses the multibase k-map equation:
// z = (~x & y) * 2 + (~x & ~y)
unsigned char *__inc_k(const __tryte(t)) {
    static __tryte(u);
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t notX = (~t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        const uint8_t notY = ~t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= (notX & y) * 2 + (notX & notY);
    }
    return u;
}

// INC gate
// Increment trit (modulo)
// xy | z
// 00 | 1
// 01 | 2
// 11 | X
// 10 | 0
// 
// This implementation uses the following equation:
// x' = y; y' = ~x ^ y
unsigned char *__inc(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t notX = ~t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= (y << 1 | notX >> 1 ^ y);
    }
    return u;
}

// DEC gate (KARNAUGH)
// Decrement trit (modulo)
// xy | z
// 00 | 2
// 01 | 0
// 11 | X
// 10 | 1
//
// This implementation uses the multibase k-map equation:
// z = (~x & ~y) * 2 + (x & ~y)
unsigned char *__dec_k(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = (t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t notX = (~t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t notY = ~t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= (notX & notY) * 2 + (x & notY);
    }
    return u;
}

// DEC gate
// Decrement trit (modulo)
// xy | z
// 00 | 2
// 01 | 0
// 11 | X
// 10 | 1
//
// This implementation uses the following equation:
// x' = x ^ ~y; y' = x
unsigned char *__dec(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t notY = ~t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= (x ^ notY << 1 | x >> 1);
    }
    return u;
}

// ISF gate (KARNAUGH)
// Check if trit is false
// xy | z
// 00 | 2
// 01 | 0
// 11 | X
// 10 | 0
//
// This implementation uses the multibase k-map equation:
// z = (~x & ~y) * 2
unsigned char *__isf_k(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t notX = (~t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t notY = ~t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= (notX & notY) * 2;
    }
    return u;
}

// ISF gate
// Check if trit is false
// xy | z
// 00 | 2
// 01 | 0
// 11 | X
// 10 | 0
//
// This implementation uses the following equation:
// x' = ~x ^ y; y' = 0
unsigned char *__isf(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t notX = ~t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= notX ^ y << 1;
    }
    return u;
}

// ISU gate (KARNAUGH)
// Check if trit is unknown
// xy | z
// 00 | 0
// 01 | 2
// 11 | X
// 10 | 0
//
// This implementation uses the multibase k-map equation:
// z = (~x & y) * 2
unsigned char *__isu_k(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = (t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t notX = (~t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= (notX & y) * 2;
    }
    return u;
}

// ISU gate
// Check if trit is unknown
// xy | z
// 00 | 0
// 01 | 2
// 11 | X
// 10 | 0
//
// This implementation uses the following equation:
// x' = y; y' = 0
unsigned char *__isu(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= y << 1;
    }
    return u;
}

// IST gate (KARNAUGH)
// Check if trit is true
// xy | z
// 00 | 0
// 01 | 0
// 11 | X
// 10 | 2
//
// This implementation uses the multibase k-map equation:
// z = (x & ~y) * 2
unsigned char *__ist_k(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = (t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t notY = ~t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= (x & notY) * 2;
    }
    return u;
}

// IST gate
// Check if trit is true
// xy | z
// 00 | 0
// 01 | 0
// 11 | X
// 10 | 2
//
// This implementation uses the following equation:
// x' = x; y' = 0
unsigned char *__ist(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        u[__byte_of_trit(i)] &= ~(0b01 << offset);
    }
    return u;
}

// CLD gate (KARNAUGH)
// Clamp down trit
// xy | z
// 00 | 0
// 01 | 1
// 11 | X
// 10 | 1
//
// This implementation uses the multibase k-map equation:
// z = x + y
unsigned char *__cld_k(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = (t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= x + y;
    }
    return u;
}

// CLD gate
// Clamp down trit
// xy | z
// 00 | 0
// 01 | 1
// 11 | X
// 10 | 1
//
// This implementation uses the following equation:
// x' = 0; y' = x ^ y
unsigned char *__cld(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= x >> 1 ^ y;
    }
    return u;
}

// CLU gate (KARNAUGH)
// Clamp up trit
// xy | z
// 00 | 1
// 01 | 1
// 11 | X
// 10 | 2
//
// This implementation uses the multibase k-map equation:
// z = (x & ~y) * 2 + ~x
unsigned char *__clu_k(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = (t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t notX = (~t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t notY = ~t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= (x & notY) * 2 + notX;
    }
    return u;
}

// CLU gate
// Clamp up trit
// xy | z
// 00 | 1
// 01 | 1
// 11 | X
// 10 | 2
//
// This implementation uses the following equation:
// x' = x; y' = ~x
unsigned char *__clu(const __tryte(t)) {
    static __tryte(u);  
    for(uint8_t i = 0; i < TRYTE_BYTE; i++) u[i] = t[i];
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t notX = ~t[__byte_of_trit(i)] & 0b10 << offset;
        u[__byte_of_trit(i)] &= ~(0b01 << offset);
        u[__byte_of_trit(i)] |= notX >> 1;
    }
    return u;
}