#ifndef LIMITS_H
#define LIMITS_H
#include <limits.h>
#endif

#ifndef MATH_H
#define MATH_H
#include "math.h"
#endif

typedef enum {
    TWA = 0b00,
    TWE = 0b01,
    TWI = 0b10
} UTRIT;

typedef enum {
    tFALSE      = 0b00,
    tUNKNOWN    = 0b01,
    tTRUE       = 0b10
} BTRIT;

// 1 trit = 2 bits
#define TRIT_BIT CEIL(3, 2)

// 1 tryte = 9 trits
#define TRYTE_TRIT 9

// 1 word = 27 trits
#define WORD_TRIT 27

// 1 byte = 4 trits
#define BYTE_TRIT (CHAR_BIT / TRIT_BIT)

// 1 tryte = 3 bytes
#define TRYTE_BYTE (CEIL(TRYTE_TRIT, BYTE_TRIT))

// 1 word = 3 trytes
#define TRYTE_WORD (CEIL(WORD_TRIT, TRYTE_TRIT))

// 1 word = 9 bytes
#define WORD_BYTE (CEIL(WORD_TRIT, BYTE_TRIT))

// 1 heptavintimal character = 3 trits
#define HEPTA_TRIT (27 / TRYTE_TRIT)

// 1 Kitri (Kt) = 3^7 = 2187
#define KITRI 2187

// 1 Metri (Mt) = 3^7^2 = 2187^2 = 4782969
#define METRI 4782969

// 1 Gitri (Gt) = 3^7^3 = 2187^3 = 10460353203
#define GITRI 10460353203

// 1 Tetri (Tt) = 3^7^4 = 2187^4 = 22876792454961
#define TETRI 22876792454961

// Page size in bytes = 4096
#define PAGE_BYTE 0x1000

// Page size in trytes = 1820
#define PAGE_TRYTE PAGE_BYTE * BYTE_TRIT / TRYTE_TRIT

// Tryte macro declaration
#define __tryte(name) uint8_t name[TRYTE_BYTE]

// Tryte buffer macro declaration
#define __tryte_buffer(name, count) uint8_t name[CEIL(count * TRYTE_TRIT, BYTE_TRIT)]

// Tryte pointer without name (return type) macro declaration
#define __tryte_ret uint8_t*

// Tryte pointer macro declaration
#define __tryte_ptr(name) uint8_t *name

// Word macro declaration
#define __word(name) uint8_t name[WORD_BYTE]

// Byte a trit is in
#define __byte_of_trit(t) ((t)*TRIT_BIT / CHAR_BIT)

// Offset of 0b11 inside a byte
#define __trit_offset(i) ((BYTE_TRIT - 1 - (i) % BYTE_TRIT) * TRIT_BIT)

// A 0 valued tryte
__tryte(tryteEMPTY) = {0b00000000, 0b00000000, 0b00000000};

// A 19683 valued tryte
__tryte(tryteFULL) = {0b10101010, 0b10101010, 0b10000000};

// Convert byte to tryte
__tryte_ret uint8_to_tryte(uint8_t n) {
    static __tryte(t);
    uint8_t i = 0, l = TRYTE_TRIT - 1;
    while(n) {
        t[__byte_of_trit(l - i)] &= ~(0b11 << __trit_offset(l - i));
        t[__byte_of_trit(l - i)] |= (n % 3) << __trit_offset(l - i);
        n /= 3; i++;
    }
    return t;
}

// Convert 4 bytes to word
__tryte_ret uint64_to_word(uint64_t n) {
    static __word(t);
    uint8_t i = 0;
    while(n) {
        uint8_t offset =  __trit_offset(WORD_TRIT - 1 - i);
        t[__byte_of_trit(WORD_TRIT - 1 - i)] &= ~(0b11 << offset);
        t[__byte_of_trit(WORD_TRIT - 1 - i)] |= (n % 3) << offset;
        n /= 3; i++;
    }
    return t;
}

// NOT gate (KARNAUGH)
// xy | z
// 00 | 2
// 01 | 1
// 11 | X
// 10 | 0
//
// This implementation uses the multibase k-map equation:
// z = (~x & ~y) * 2 + (~x & y)
unsigned char *__not_k(const __tryte(t))
{
    static __tryte(u);
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
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
// xy | z
// 00 | 2
// 01 | 1
// 11 | X
// 10 | 0
//
// This implementation uses the following equation:
// x' = ~x ^ y; y' = y
unsigned char *__not(const __tryte(t)) {
    static __tryte(u);
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t notX = ~t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= notX ^ y << 1 | y;
    }
    return u;
}

// INC gate (KARNAUGH)
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
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
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
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t notX = ~t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= (y << 1 | notX >> 1 ^ y);
    }
    return u;
}

// DEC gate (KARNAUGH)
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
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
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
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t notY = ~t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= (x ^ notY << 1 | x >> 1);
    }
    return u;
}

// ISF gate (KARNAUGH)
// xy | z
// 00 | 2
// 01 | 0
// 11 | X
// 10 | 0
//
// This implementation uses the multibase k-map equation:
// z = ~(x | y) * 2
unsigned char *__isf_k(const __tryte(t)) {
    static __tryte(u);
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t notX = (~t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t notY = ~t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= (notX & notY) * 2;
    }
    return u;
}

// ISF gate
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
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t notX = ~t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= notX ^ y << 1;
    }
    return u;
}

// ISU gate (KARNAUGH)
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
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
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
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= y << 1;
    }
    return u;
}

// IST gate (KARNAUGH)
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
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = (t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t notY = ~t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= (x & notY) * 2;
    }
    return u;
}

// IST gate
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
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = (t[__byte_of_trit(i)] & 0b10 << offset);
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= x;
    }
    return u;
}

// CLD gate (KARNAUGH)
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
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = (t[__byte_of_trit(i)] & 0b10 << offset) >> 1;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= x + y;
    }
    return u;
}

// CLD gate
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
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= x >> 1 ^ y;
    }
    return u;
}

// CLU gate (KARNAUGH)
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
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
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
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t notX = ~t[__byte_of_trit(i)] & 0b10 << offset;
        u[__byte_of_trit(i)] &= ~(0b11 << offset);
        u[__byte_of_trit(i)] |= x | notX >> 1;
    }
    return u;
}

// MIN or AND gate
// zw  00 01 11 10
// xy  -----------
// 00 | 0  0  X  0
// 01 | 0  1  X  1
// 11 | X  X  X  X
// 10 | 0  1  X  2
//
// This implementation uses the following equation:
// x' = x & z; y' = ((x ^ y) & (z ^ w) & ~(x & z))
unsigned char *__and(const __tryte(t), const __tryte(u)) {
    static __tryte(v);
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        const uint8_t z = u[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t w = u[__byte_of_trit(i)] & 0b01 << offset;
        v[__byte_of_trit(i)] &= ~(0b11 << offset);
        v[__byte_of_trit(i)] |= x & z | ((x >> 1 ^ y) & (z >> 1 ^ w) & ~(x & z) >> 1);
    }
    return v;
}

// MAX or OR gate
// zw  00 01 11 10
// xy  -----------
// 00 | 0  1  X  2
// 01 | 1  1  X  2
// 11 | X  X  X  X
// 10 | 2  2  x  2
//
// This implementation uses the following equation:
// x' = x | z; y' = (y | w) & ~(z ^ x)
unsigned char *__or(const __tryte(t), const __tryte(u)) {
    static __tryte(v);
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        const uint8_t z = u[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t w = u[__byte_of_trit(i)] & 0b01 << offset;
        v[__byte_of_trit(i)] &= ~(0b11 << offset);
        v[__byte_of_trit(i)] |= x | z | ((y | w) & ~(z >> 1 ^ x >> 1));
    }
    return v;
}

// NMIN or NAND
// zw  00 01 11 10
// xy  -----------
// 00 | 2  2  X  2
// 01 | 2  1  X  1
// 11 | X  X  X  X
// 10 | 2  1  X  0
//
// This implementation uses the following equation:
// x' = ~y' & ~(x & z); y' = y & (z | w) | x & w
unsigned char *__nand(const __tryte(t), const __tryte(u)) {
    static __tryte(v);
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        const uint8_t z = u[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t w = u[__byte_of_trit(i)] & 0b01 << offset;
        const uint8_t yL = (y & (z >> 1 | w) | x >> 1 & w) & 0b01 << offset;
        const uint8_t notYL = ~yL & 0b01 << offset;
        v[__byte_of_trit(i)] &= ~(0b11 << offset);
        v[__byte_of_trit(i)] |= notYL << 1 & ~(x & z) | yL;
    }   
    return v;
}

// NMAX or NOR
// zw  00 01 11 10
// xy  -----------
// 00 | 2  1  X  0
// 01 | 1  1  X  0
// 11 | X  X  X  X
// 10 | 0  0  X  0
//
// This implementation uses the following equation:
// x' = ~(x | y | z | w); y' = ~(x ^ z) & (y | w)
unsigned char *__nor(const __tryte(t), const __tryte(u)) {
    static __tryte(v);
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        const uint8_t z = u[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t w = u[__byte_of_trit(i)] & 0b01 << offset;
        v[__byte_of_trit(i)] &= ~(0b11 << offset);
        v[__byte_of_trit(i)] |= ~(x | y << 1 | z | w << 1) & 0b10 << offset
        | ~(x ^ z) >> 1 & (y | w);
    }
    return v;
}

// XOR
// zw  00 01 11 10
// xy  -----------
// 00 | 0  1  X  2
// 01 | 1  1  X  1
// 11 | X  X  X  X
// 10 | 2  1  X  0
//
// This implementation uses the following equation:
// x' = x & (~z ^ w) | z & (~x ^ y); y' = w | y
unsigned char *__xor(const __tryte(t), const __tryte(u)) {
    static __tryte(v);
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t y = t[__byte_of_trit(i)] & 0b01 << offset;
        const uint8_t z = u[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t w = u[__byte_of_trit(i)] & 0b01 << offset;
        v[__byte_of_trit(i)] &= ~(0b11 << offset);
        v[__byte_of_trit(i)] |= x & (~z ^ w << 1) | z & (~x ^ y << 1)
        | w | y;
    }
    return v;
}

// SUM
// zw  00 01 11 10
// xy  -----------
// 00 | 2  0  X  1
// 01 | 0  1  X  2
// 11 | X  X  X  X
// 10 | 1  2  X  0
//
// This implementation uses the following equation:
// x' = (~y ^ z) & (~x ^ w); y' = (~y ^ w) & (x ^ z)
unsigned char *__sum(const __tryte(t), const __tryte(u)) {
    static __tryte(v);
    for (uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t offset = __trit_offset(i);
        const uint8_t x = t[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t notY = ~t[__byte_of_trit(i)] & 0b01 << offset;
        const uint8_t z = u[__byte_of_trit(i)] & 0b10 << offset;
        const uint8_t w = u[__byte_of_trit(i)] & 0b01 << offset;
        v[__byte_of_trit(i)] &= ~(0b11 << offset);
        v[__byte_of_trit(i)] |= (notY << 1 ^ z) & (~x ^ w << 1)
        | (notY ^ w) & (x ^ z) >> 1;
    }
    return v;
}