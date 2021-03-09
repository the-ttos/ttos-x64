#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
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

// Get full memory size in bytes
uint64_t get_memory_size(EFI_MEMORY_DESCRIPTOR *map, uint64_t mapEntries, uint64_t mapDescriptorSize) {
    static uint64_t memorySize = 0;
    if(memorySize) return memorySize;
    
    for(uint64_t i = 0; i < mapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR *descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + (i * mapDescriptorSize));
        memorySize += descriptor->pageCount * PAGE_BYTE;
    }

    return memorySize;
}

// Byte of last tryte of specified byte
__tryte_ret tryte_a(void *t) {
    return (__tryte_ret)((uint64_t)t
        + ((uint64_t)t % TRIT_BIT != (uint64_t)t / TRYTE_TRIT % TRIT_BIT)
        + ((uint64_t)t % TRYTE_TRIT >= TRYTE_TRIT - TRIT_BIT));
}

// Set a tryte in memory to a value
void tryteset(void *address, __tryte(t)) {
    uint64_t byte = (uint64_t)tryte_a(address);
    uint8_t offset = byte % TRYTE_TRIT;
    uint8_t mask = 0xff >> offset;
    ((__tryte_ret)address)[0] &= ~mask;
    ((__tryte_ret)address)[0] |= t[0] >> offset;
    ((__tryte_ret)address)[1] = 0;
    ((__tryte_ret)address)[1] |= t[0] << CHAR_BIT - offset;
    ((__tryte_ret)address)[1] |= t[1] >> offset;
    mask >>= TRIT_BIT;
    ((__tryte_ret)address)[2] &= mask;
    ((__tryte_ret)address)[2] |= t[1] << CHAR_BIT - offset;
    ((__tryte_ret)address)[2] |= (t[2] & 0b11000000) >> offset; // 0b11000000 = Last byte mask for normal tryte
}

// Copy the value of a tryte in memory to another tryte in memory
void trytecpy(void* from, void *to) {
    from = tryte_a(from);
    to = tryte_a(to);
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
    maskTo >>= TRIT_BIT;
    maskFrom >>= TRIT_BIT;
    ((__tryte_ret)to)[1] |= (((__tryte_ret)from)[2] & ~maskFrom) >> CHAR_BIT - offsetFrom - offsetTo;
    ((__tryte_ret)to)[2] &= maskTo;
    ((__tryte_ret)to)[2] |= ((__tryte_ret)from)[1] << CHAR_BIT + offsetFrom - offsetTo;
    ((__tryte_ret)to)[2] |= (((__tryte_ret)from)[2] & ~maskFrom) << offsetFrom - offsetTo;
}

// Print trits in provided address
const char *memview(__tryte_ptr(address), uint8_t num) {
    static char bitBuffer[256 * 9 + 1]; // TEMP LENGTH
    uint32_t p = 0;
    for(uint8_t i = 0; i < num; i++) {
        uint64_t byte = ((uint64_t)address + i);
        bitBuffer[p++] = ':';
        bitBuffer[p++] = ' ';
        for(uint8_t j = 0; j < CHAR_BIT; j++) {
            if(byte % TRIT_BIT == byte / TRYTE_TRIT % TRIT_BIT - (byte % TRYTE_TRIT == TRYTE_TRIT - 1)
                && j == byte % TRYTE_TRIT) {
                bitBuffer[p++] = '|';
                bitBuffer[p++] = ' ';
            }
            bitBuffer[p++] = (*(uint8_t*)byte & (1 << (CHAR_BIT - 1 - j))) ? '1' : '0';
            if(j % 2) bitBuffer[p++] = ' ';
        }
    }
    bitBuffer[p] = '\0';
    return bitBuffer;
}

// Set trytes in memory
void memset(__tryte_ptr(address), __tryte(value), uint64_t num) {
    address = tryte_a((__tryte_ret)address);
    for(uint64_t i = 0; i < num; i++) tryteset(address + i * 2, value);
}