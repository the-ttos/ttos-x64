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

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

// Tritmap struct
typedef struct {
    size_t size;
    __tryte_ptr(buffer);
} TRITMAP;

// Read trit from tritmap
BTRIT read_trit(TRITMAP *t, uint64_t index) {
    const uint64_t offset = __trit_offset(index);
    return (BTRIT)((t->buffer[__byte_of_trit(index)]
        & 0b11 << offset) >> offset);
}

// Write trit to tritmap at index
bool write_trit(TRITMAP *t, uint64_t index, BTRIT trit) {
    if(index >= t->size) return false;
    const uint64_t offset = __trit_offset(index);
    const uint64_t byte = __byte_of_trit(index);
    t->buffer[byte] &= ~(0b11 << offset);
    t->buffer[byte] |= (trit & 0b11) << offset;
    return true;
}