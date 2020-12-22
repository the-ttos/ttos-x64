#include <stdint.h>

char uintBuffer[128];
const char *to_string(uint64_t value) {
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
    uintBuffer[size + 1] = 0;
    return uintBuffer;
}