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
uint8_t initialized = 0;
TRITMAP pageTritmap;

void init_tritmap(size_t bytes, void *bufferAddress, uint8_t extraTrytes) {
    pageTritmap.size = bytes / 8 * 4 + extraTrytes;
    pageTritmap.buffer = (uint8_t*)bufferAddress;
    for(uint8_t i = 0; i < pageTritmap.size; i++) *(uint8_t*)(pageTritmap.buffer + i) = 0;
}

void free_page(void *address) {
    uint64_t index = (uint64_t)address / 4096 / 2;
    if(read_trit(pageTritmap.buffer, index) != TRUE) return;
    write_trit(pageTritmap.buffer, index, FALSE);
    freeMemory += 4096;
    usedMemory -= 4096;
}

void lock_page(void *address) {
    uint64_t index = (uint64_t)address / 4096 / 2;
    if(read_trit(pageTritmap.buffer, index) == TRUE) return;
    write_trit(pageTritmap.buffer, index, TRUE);
    freeMemory -= 4096;
    usedMemory += 4096;
}

void unreserve_page(void *address) {
    uint64_t index = (uint64_t)address / 4096 / 2;
    if(read_trit(pageTritmap.buffer, index) != TRUE) return;
    write_trit(pageTritmap.buffer, index, FALSE);
    freeMemory += 4096;
    reservedMemory -= 4096;
}

void reserve_page(void *address) {
    uint64_t index = (uint64_t)address / 4096 / 2;
    if(read_trit(pageTritmap.buffer, index) == TRUE) return;
    write_trit(pageTritmap.buffer, index, TRUE);
    freeMemory -= 4096;
    reservedMemory += 4096;
}

void free_pages(void *address, uint64_t pageCount) {
    for(uint64_t i = 0; i < pageCount; i++)
        free_page((void*)((uint64_t)address + (i * 4096)));
}

void lock_pages(void *address, uint64_t pageCount) {
    for(uint64_t i = 0; i < pageCount; i++)
        lock_page((void*)((uint64_t)address + (i * 4096)));
}

void unreserve_pages(void *address, uint64_t pageCount) {
    for(uint64_t i = 0; i < pageCount; i++)
        unreserve_page((void*)((uint64_t)address + (i * 4096)));
}

void reserve_pages(void *address, uint64_t pageCount) {
    for(uint64_t i = 0; i < pageCount; i++)
        unreserve_page((void*)((uint64_t)address + (i * 4096)));
}

void read_efi_memory_map(EFI_MEMORY_DESCRIPTOR *map, size_t mapSize, size_t mapDescriptorSize) {
    if(initialized) return;
    initialized = 1;
    
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

    lock_pages(pageTritmap.buffer, pageTritmap.size / 4096 / 2 + 1);
    
    for(uint32_t i = 0; i < mapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + (i * mapDescriptorSize));
        if(desc->type != 7) // type = EfiConventionalMemory
            reserve_pages(desc->physicalAddress, desc->pageCount);
    }
}

uint64_t get_free_RAM() {
    return freeMemory / 2;   
}

uint64_t get_used_RAM() {
    return usedMemory / 2;   
}

uint64_t get_reserved_RAM() {
    return reservedMemory / 2;   
}