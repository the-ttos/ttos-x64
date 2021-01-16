#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef STDDEF_H
#define STDDEF_H
#include <stddef.h>
#endif

#ifndef TRYTE_H
#define TRYTE_H
#include "tryte.h"
#endif

// Tritmap struct
typedef struct {
    size_t size;
    __tryte_buffer_ptr(buffer);
} TRITMAP;

// Read trit from tritmap
BTRIT read_trit(__tryte_buffer_ptr(t), uint64_t index) {
    const uint8_t offset = __trit_offset(index);
    return (BTRIT)((t[__byte_of_trit(index)]
        & 0b11 << offset) >> offset);
}

// Write trit to tritmap at index
void write_trit(__tryte_buffer_ptr(t), uint64_t index, BTRIT trit) {
    const uint8_t offset = __trit_offset(index);
    const uint8_t byte = __byte_of_trit(index);
    t[byte] &= ~(0b11 << offset);
    t[byte] |= (trit & 0b11) << offset;
}