#ifndef EFIMEMORY_H
#define EFIMEMORY_H
#include "efimemory.h"
#endif

#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef TRITMAP_H
#define TRITMAP_H
#include "tritmap.h"
#endif

#ifndef MEMORY_H
#define MEMORY_H
#include "memory.h"
#endif

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
uint8_t initialized = false;
TRITMAP pageTritmap;

void read_efi_memory_map(EFI_MEMORY_DESCRIPTOR *map, size_t mapSize, size_t mapDescriptorSize) {
    if(initialized) return;
    initialized = true;
    
    uint64_t mapEntries = mapSize / mapDescriptorSize;
    void *largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;
    
    for(uint32_t i = 0; i < mapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + (i * mapDescriptorSize));
        if(desc->type == 7 && desc->pageCount * 4096 > largestFreeMemSegSize) { // type = EfiConventionalMemory
            largestFreeMemSeg = desc->physicalAddress;
            largestFreeMemSegSize = desc->pageCount * 4096;
        }
    }

    uint64_t memorySize = get_memory_size(map, mapEntries, mapDescriptorSize);
    freeMemory = memorySize;
    uint64_t tritmapBytes = memorySize / 4096 / 8;

    init_tritmap(tritmapBytes, largestFreeMemSeg, 1);

}

void init_tritmap(size_t bytes, void *bufferAddress, uint8_t extraTrytes) {
    pageTritmap.size = bytes / 8 * 4 + extraTrytes;
    pageTritmap.buffer = (uint8_t*)bufferAddress;
    for(uint8_t i = 0; i < pageTritmap.size; i++) *(uint8_t*)(pageTritmap.buffer + i) = 0;
}