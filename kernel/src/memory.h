#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef EFIMEMORY_H
#define EFIMEMORY_H
#include "efimemory.h"
#endif

uint64_t get_memory_size(EFI_MEMORY_DESCRIPTOR *map, uint64_t mapEntries, uint64_t mapDescriptorSize) {
    static uint64_t memorySize = 0;
    if(memorySize) return memorySize;

    for(uint32_t i = 0; i < mapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR *descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + (i * mapDescriptorSize));
        memorySize += descriptor->pageCount * 4096;
    }

    return memorySize;
}