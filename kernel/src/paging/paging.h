#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

typedef struct {
    bool present: 1;
    bool readWrite: 1;
    bool userSuper: 1;
    bool writeThrough: 1;
    bool cahceDisabled: 1;
    bool accessed: 1;
    bool ignore0: 1;
    bool largerPages: 1;
    bool ignore1: 1;
    uint8_t available: 3;
    uint64_t address: 52;
} PAGE_DIRECTORY_ENTRY;

// typedef struct {
//     PAGE_DIRECTORY_ENTRY entries[512];
// } __attribute__ ((aligned (PAGE_BYTE))) PAGE_TABLE;

typedef struct __attribute__ ((aligned (PAGE_BYTE))) {
    PAGE_DIRECTORY_ENTRY entries[512];
} PAGE_TABLE;