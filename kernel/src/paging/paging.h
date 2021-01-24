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
    bool readwrite: 1;
    bool userSuper: 1;
} PAGE_DIRECTORY_ENTRY;