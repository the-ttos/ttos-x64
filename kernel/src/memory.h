#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef EFIMEMORY_H
#define EFIMEMORY_H
#include "efimemory.h"
#endif

#ifndef TRYTE_H
#define TRYTE_H
#include "tryte.h"
#endif

#ifndef BOOT_H
#define BOOT_H
#include "boot.h"
#endif

uint64_t get_memory_size(EFI_MEMORY_DESCRIPTOR *map, uint64_t mapEntries, uint64_t mapDescriptorSize) {
    static uint64_t memorySize = 0;
    if(memorySize) return memorySize;
    
    for(uint64_t i = 0; i < mapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR *descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + (i * mapDescriptorSize));
        memorySize += descriptor->pageCount * PAGE_TRYTE;
    }
    return memorySize;
}

// Tryte at current specified byte
__tryte_ret tryte_c(void *t) {
    return (__tryte_ret)((uint64_t)t * BYTE_TRIT / TRYTE_TRIT * TRYTE_TRIT / BYTE_TRIT);
}

// Byte of next available tryte at specified byte
__tryte_ret tryte_n(void *t) {
    return (__tryte_ret)((uint64_t)t
        + ((uint64_t)t % TRIT_BIT != ((uint64_t)t + 1) / TRYTE_TRIT % TRIT_BIT)
        + ((uint64_t)t % TRYTE_TRIT == TRYTE_TRIT - TRIT_BIT));
}

// Byte of last tryte of specified byte
__tryte_ret tryte_l(void *t) {
    return (__tryte_ret)((uint64_t)t
        - ((uint64_t)t % TRIT_BIT != (uint64_t)t / TRYTE_TRIT % TRIT_BIT)
        - ((uint64_t)t % TRYTE_TRIT == TRYTE_TRIT - 1));
}

// Set a tryte in memory to a value
void tryteset(void *address, __tryte(t)) {
    uint8_t byte = tryte_l(address);
    uint8_t offset = byte % TRYTE_TRIT;
    uint8_t mask = 0xff >> offset;
    ((__tryte_ret)address)[0] &= ~mask;
    ((__tryte_ret)address)[0] |= t[0] >> offset;
    ((__tryte_ret)address)[1] = 0;
    ((__tryte_ret)address)[1] |= t[0] << CHAR_BIT - offset;
    ((__tryte_ret)address)[1] |= t[1] >> offset;
    mask >>= 2;
    ((__tryte_ret)address)[2] &= mask;
    ((__tryte_ret)address)[2] |= t[1] << CHAR_BIT - offset;
    ((__tryte_ret)address)[2] |= (t[2] & 0b11000000) >> offset; // 0b11000000 = Last byte mask for normal tryte
}

// Copy the value of a tryte in memory to another tryte in memory
void trytecpy(void *from, void *to) {
    from = tryte_c(from);
    to = tryte_c(to);
    uint8_t offsetFrom = (uint64_t)from * BYTE_TRIT / TRYTE_TRIT % BYTE_TRIT * TRIT_BIT;
    uint8_t offsetTo = (uint64_t)to * BYTE_TRIT / TRYTE_TRIT % BYTE_TRIT * TRIT_BIT;
    uint8_t maskTo = 0xff >> offsetTo;
    uint8_t maskFrom = 0xff >> offsetFrom;
    ((__tryte_ret)to)[0] &= ~maskTo;
    ((__tryte_ret)to)[0] |= (((__tryte_ret)from)[0] & maskFrom) << offsetFrom - offsetTo;
    ((__tryte_ret)to)[0] |= ((__tryte_ret)from)[1] >> CHAR_BIT - offsetFrom - offsetTo;
    ((__tryte_ret)to)[1] = 0;
    ((__tryte_ret)to)[1] |= (((__tryte_ret)from)[0] & maskFrom) << CHAR_BIT + offsetFrom - offsetTo;
    ((__tryte_ret)to)[1] |= ((__tryte_ret)from)[1] << offsetFrom - offsetTo;
    maskTo >>= 2;
    maskFrom >>= 2;
    ((__tryte_ret)to)[1] |= (((__tryte_ret)from)[2] & ~maskFrom) >> CHAR_BIT - offsetFrom - offsetTo;
    ((__tryte_ret)to)[2] &= maskTo;
    ((__tryte_ret)to)[2] |= ((__tryte_ret)from)[1] << CHAR_BIT + offsetFrom - offsetTo;
    ((__tryte_ret)to)[2] |= (((__tryte_ret)from)[2] & ~maskFrom) << offsetFrom - offsetTo;
}

void memset(void *address, __tryte(value), uint64_t num) {
    address = tryte_l((__tryte_ret)address);
    num *= TRYTE_TRIT * TRIT_BIT / CHAR_BIT;
    for(; (uint64_t)address < num; address += TRIT_BIT
        + ((uint64_t)address % TRYTE_TRIT == TRYTE_TRIT - TRYTE_BYTE))
        tryteset(address, value);
}