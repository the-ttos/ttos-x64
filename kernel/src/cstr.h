#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef TRYTE_H
#define TRYTE_H
#include "tryte.h"
#endif

#ifndef MEMORY_H
#define MEMORY_H
#include "memory.h"
#endif


// Unsigned 8 bytes integer to string conversion
const char *uint64_to_string(uint64_t value) {
    static char uintBuffer[128];
    uint8_t size;
    uint64_t test = value;
    while(test / 10) {
        test /= 10;
        size++;
    }

    uint8_t index = 0;
    while(value / 10) {
        uint8_t remainder = value % 10;
        value /= 10;
        uintBuffer[size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    uintBuffer[size - index] = remainder + '0';
    uintBuffer[size + 1] = '\0';
    return uintBuffer;
}

// Tryte (3 bytes) integer to string
const char *tryte_to_string(__tryte(t)) {
    uint64_t result = 0;
    for(uint8_t i = 0; i < TRYTE_TRIT; i++)
        result += ((t[__byte_of_trit(i)]
        & (0b11 << (BYTE_TRIT - 1 - i % BYTE_TRIT) * TRIT_BIT))
        >> (BYTE_TRIT - 1 - i % BYTE_TRIT) * TRIT_BIT)
        * power_uint8(3, TRYTE_TRIT - 1 - i);
    return uint64_to_string(result);
}

// Word (9 bytes) integer to string
const char *word_to_string(__word(t)) {
    uint64_t result = 0;
    for(uint8_t i = 0; i < WORD_TRIT; i++)
        result += ((t[__byte_of_trit(i)]
        & (0b11 << (BYTE_TRIT - 1 - i % BYTE_TRIT) * TRIT_BIT))
        >> (BYTE_TRIT - 1 - i % BYTE_TRIT) * TRIT_BIT)
        * power_uint8(3, WORD_TRIT - 1 - i);
    return uint64_to_string(result);
}

// Tryte (3 bytes) integer to ternary string
const char *tryte_to_tstring(__tryte(t)) {
    static char tryteBuffer[2 + TRYTE_TRIT + 1];
    tryteBuffer[0] = '0';
    tryteBuffer[1] = 't';
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        tryteBuffer[2 + i] = '0' + ((t[__byte_of_trit(i)]
        & (0b11 << (BYTE_TRIT - 1 - i % BYTE_TRIT) * TRIT_BIT))
        >> (BYTE_TRIT - 1 - i % BYTE_TRIT) * TRIT_BIT);
    }
    tryteBuffer[2 + TRYTE_TRIT] = '\0';
    return tryteBuffer;
}

// Word (9 bytes) integer to ternary string
const char *word_to_tstring(__word(t)) {
    static char wordBuffer[2 + WORD_TRIT + 1];
    wordBuffer[0] = '0';
    wordBuffer[1] = 't';
    for(uint8_t i = 0; i < WORD_TRIT; i++) {
        wordBuffer[2 + i] = '0' + ((t[__byte_of_trit(i)]
        & (0b11 << (BYTE_TRIT - 1 - i % BYTE_TRIT) * TRIT_BIT))
        >> (BYTE_TRIT - 1 - i % BYTE_TRIT) * TRIT_BIT);
    }
    wordBuffer[2 + WORD_TRIT] = '\0';
    return wordBuffer;
}

// Tryte (3 bytes) integer to heptavintimal string
const char *tryte_to_hstring(__tryte(t)) {
    static char tryteHeptaBuffer[2 + HEPTA_TRIT + 1];
    tryteHeptaBuffer[0] = '0';
    tryteHeptaBuffer[1] = 'h';
    for(uint8_t i = 0; i < TRYTE_TRIT; i += 3) {
        const uint8_t index = 2 + i / HEPTA_TRIT;
        // 0tX00 +
        tryteHeptaBuffer[index] = ((t[__byte_of_trit(i + 0)] & 0b11
            << (BYTE_TRIT - 1 - (i + 0) % BYTE_TRIT) * TRIT_BIT)
            >> (BYTE_TRIT - 1 - (i + 0) % BYTE_TRIT) * TRIT_BIT) * 9 // 3 to the power of 2
        // 0t0X0 +
                            + ((t[__byte_of_trit(i + 1)] & 0b11
            << (BYTE_TRIT - 1 - (i + 1) % BYTE_TRIT) * TRIT_BIT)
            >> (BYTE_TRIT - 1 - (i + 1) % BYTE_TRIT) * TRIT_BIT) * 3 // 3 to the power of 1
        // 0t00X
                            + ((t[__byte_of_trit(i + 2)] & 0b11
            << (BYTE_TRIT - 1 - (i + 2) % BYTE_TRIT) * TRIT_BIT)
            >> (BYTE_TRIT - 1 - (i + 2) % BYTE_TRIT) * TRIT_BIT) * 1; // 3 to the power of 0
        tryteHeptaBuffer[index] += '0' + (tryteHeptaBuffer[index] >= 10) * ('A' - '9' - 1);
    }
    tryteHeptaBuffer[2 + HEPTA_TRIT] = '\0';
    return tryteHeptaBuffer;
}

// Word (9 bytes) integer to heptavintimal string
const char *word_to_hstring(__tryte(t)) {
    static char wordHeptaBuffer[2 + HEPTA_TRIT * TRYTE_WORD + 1];
    wordHeptaBuffer[0] = '0';
    wordHeptaBuffer[1] = 'h';
    for(uint8_t i = 0; i < WORD_TRIT; i += 3) {
        const uint8_t index = 2 + i / HEPTA_TRIT;
        // 0tX00 +
        wordHeptaBuffer[index] = ((t[__byte_of_trit(i + 0)] & 0b11
            << (BYTE_TRIT - 1 - (i + 0) % BYTE_TRIT) * TRIT_BIT)
            >> (BYTE_TRIT - 1 - (i + 0) % BYTE_TRIT) * TRIT_BIT) * 9 // 3 to the power of 2
        // 0t0X0 +
                            + ((t[__byte_of_trit(i + 1)] & 0b11
            << (BYTE_TRIT - 1 - (i + 1) % BYTE_TRIT) * TRIT_BIT)
            >> (BYTE_TRIT - 1 - (i + 1) % BYTE_TRIT) * TRIT_BIT) * 3 // 3 to the power of 1
        // 0t00X
                            + ((t[__byte_of_trit(i + 2)] & 0b11
            << (BYTE_TRIT - 1 - (i + 2) % BYTE_TRIT) * TRIT_BIT)
            >> (BYTE_TRIT - 1 - (i + 2) % BYTE_TRIT) * TRIT_BIT) * 1; // 3 to the power of 0
        wordHeptaBuffer[index] += '0' + (wordHeptaBuffer[index] >= 10) * ('A' - '9' - 1);
    }
    wordHeptaBuffer[2 + HEPTA_TRIT * TRYTE_WORD] = '\0';
    return wordHeptaBuffer;
}

// Trit (2 bits) to balanced trit string
const char *trit_to_bstring(BTRIT t) {
    switch(t) {
        case 0: return "FALSE";
        case 1: return "UNKNOWN";
        case 2: return "TRUE";
    }
    return 0;
}