#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef TRYTE_H
#define TRYTE_H
#include "tryte.h"
#endif

// Unsigned 8 bytes integer to string conversion
char uintBuffer[128];
const char *uint64_to_string(uint64_t value) {
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
        result += ((t[__trit_byte(i)]
        & (0b11 << (BYTE_TRIT - 1 - i % BYTE_TRIT) * TRIT_BIT))
        >> (BYTE_TRIT - 1 - i % BYTE_TRIT) * TRIT_BIT)
        * power_uint8(3, TRYTE_TRIT - 1 - i);
    return uint64_to_string(result);
}

// Tryte (3 bytes) integer to ternary string
char tryteBuffer[2 + TRYTE_TRIT + 1];
const char *tryte_to_tstring(__tryte(t)) {
    tryteBuffer[0] = '0';
    tryteBuffer[1] = 't';
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        tryteBuffer[2 + i] = '0' + ((t[__trit_byte(i)]
        & (0b11 << (BYTE_TRIT - 1 - i % BYTE_TRIT) * TRIT_BIT))
        >> (BYTE_TRIT - 1 - i % BYTE_TRIT) * TRIT_BIT);
    }
    tryteBuffer[2 + TRYTE_TRIT] = '\0';
    return tryteBuffer;
}
