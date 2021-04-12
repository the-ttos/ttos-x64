#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef PAGING_H
#define PAGING_H
#include "paging/paging.h"
#endif

typedef struct {
    uint16_t size;
    uint64_t offset;
}__attribute__((packed)) GDT_DESCRIPTOR;

typedef struct {
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t accessByte;
    uint8_t limit1_flags;
    uint8_t base2;
}__attribute__((packed)) GDT_ENTRY;

typedef struct {
    GDT_ENTRY null;         // 0x00
    GDT_ENTRY kernelCode;   // 0x08
    GDT_ENTRY kernelData;   // 0x10
    GDT_ENTRY userNull;
    GDT_ENTRY userCode;
    GDT_ENTRY userData;
}__attribute__((packed, aligned(PAGE_BYTE))) GDT;

extern GDT defaultGDT;
extern void LoadGDT(GDT_DESCRIPTOR *gdtDescriptor);

__attribute__((aligned(PAGE_BYTE)))
GDT defaultGDT = {
    { 0, 0, 0, 0x00, 0x00, 0 }, // null
    { 0, 0, 0, 0x9a, 0xa0, 0 }, // kernelCode
    { 0, 0, 0, 0x92, 0xa0, 0 }, // kernelData
    { 0, 0, 0, 0x00, 0x00, 0 }, // userNull
    { 0, 0, 0, 0x9a, 0xa0, 0 }, // userCode
    { 0, 0, 0, 0x92, 0xa0, 0 }, // userData
};
