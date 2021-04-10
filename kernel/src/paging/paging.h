#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

// Page Directory Entry structure (MMU)
typedef struct {
    bool present: 1;
    bool readWrite: 1;
    bool userSuper: 1;
    bool writeThrough: 1;
    bool cacheDisabled: 1;
    bool accessed: 1;
    bool ignore0: 1;
    bool largerPages: 1;
    bool ignore1: 1;
    uint8_t available: 3;
    uint64_t address: 52;
} PAGE_DIRECTORY_ENTRY;

// Page Table structure
typedef struct {
    PAGE_DIRECTORY_ENTRY entries[512];
} __attribute__ ((aligned (PAGE_BYTE))) PAGE_TABLE;