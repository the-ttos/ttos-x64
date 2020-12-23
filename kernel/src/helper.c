#include "cstr.h"
#include <stdio.h>

// Tryte (3 bytes) integer to heptavintimal string
char heptaBuffer[2 + HEPTA_TRYTE + 1];
const char *tryte_to_hstring(__tryte(t)) {
    heptaBuffer[0] = '0';
    heptaBuffer[1] = 'h';
    for(uint8_t i = 0; i < TRYTE_TRIT; i++) {
        const uint8_t byte = 2 + i / HEPTA_TRYTE;
        const uint8_t trit = BYTE_TRIT - 1 - i % BYTE_TRIT;
        if(i % HEPTA_TRYTE == 0)
            heptaBuffer[byte] = 0;
        heptaBuffer[byte] += ((t[__trit_byte(i)]
        & (0b11 << trit * TRIT_BIT)) >> trit * TRIT_BIT)
        * power_uint8(3, trit);
        printf("%li\n", ((t[__trit_byte(i)]
        & (0b11 << trit * TRIT_BIT)) >> trit * TRIT_BIT)
        * power_uint8(3, trit));
        if(i % HEPTA_TRYTE == 2) {
            if(heptaBuffer[byte] < 10)
                heptaBuffer[byte] += 'A' - '9' - 1;
            heptaBuffer[byte] += '0';
        }
    }
    heptaBuffer[2 + HEPTA_TRYTE] = '\0';
    return heptaBuffer;
}

int main() {
    __tryte(t) = {0b01000000, 0b00000000, 0b00000000};
    tryte_to_hstring(t);
}