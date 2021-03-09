#ifndef EFIMEMORY_H
#define EFIMEMORY_H
#include "efimemory.h"
#endif

#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef BITMAP_H
#define BITMAP_H
#include "../bitmap.h"
#endif

#ifndef TRITMAP_H
#define TRITMAP_H
#include "../tritmap.h"
#endif

#ifndef MEMORY_H
#define MEMORY_H
#include "../memory.h"
#endif

#ifndef MATH_H
#define MATH_H
#include "../math.h"
#endif

#ifndef TRYTE_H
#define TRYTE_H
#include "../tryte.h"
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool initialized = false;
TRITMAP pageTritmap;

// Initialize the pages Tritmap
void init_page_tritmap(size_t size, void *bufferAddress) {
    pageTritmap.size = size;
    pageTritmap.buffer = (uint8_t*)bufferAddress;
    for(size_t i = 0; i < ceil(pageTritmap.size, BYTE_TRIT); i++) *(uint8_t*)(pageTritmap.buffer + i) = 0;
}

// Free a page (set it's trit to tFALSE)
void free_page(void *address) {
    uint64_t index = (uint64_t)address / PAGE_BYTE;
    if(read_trit(pageTritmap.buffer, index) == tFALSE) return;
    write_trit(pageTritmap.buffer, index, tFALSE);
    freeMemory += PAGE_TRYTE;
    usedMemory -= PAGE_TRYTE;
}

// Lock a page (set it's trit to tTRUE)
void lock_page(void *address) {
    uint64_t index = (uint64_t)address / PAGE_BYTE;
    if(read_trit(pageTritmap.buffer, index) == tTRUE) return;
    write_trit(pageTritmap.buffer, index, tTRUE);
    freeMemory -= PAGE_TRYTE;
    usedMemory += PAGE_TRYTE;
}

// Unreserve a page (set it's trit to tFALSE)
void unreserve_page(void *address) {
    uint64_t index = (uint64_t)address / PAGE_BYTE;
    if(read_trit(pageTritmap.buffer, index) == tFALSE) return;
    write_trit(pageTritmap.buffer, index, tFALSE);
    freeMemory += PAGE_TRYTE;
    reservedMemory -= PAGE_TRYTE;
}

// Reserve a page (set it's trit to tUNKNOWN)
void reserve_page(void *address) {
    uint64_t index = (uint64_t)address / PAGE_BYTE;
    if(read_trit(pageTritmap.buffer, index) == tUNKNOWN) return;
    write_trit(pageTritmap.buffer, index, tUNKNOWN);
    freeMemory -= PAGE_TRYTE;
    reservedMemory += PAGE_TRYTE;
}

// Free multiple pages
void free_pages(void *address, uint64_t pageCount) {
    for(uint64_t i = 0; i < pageCount; i++)
        free_page((void*)((uint64_t)address + i * PAGE_BYTE));
}

// Lock multiple pages
void lock_pages(void *address, uint64_t pageCount) {
    for(uint64_t i = 0; i < pageCount; i++)
        lock_page((void*)((uint64_t)address + i * PAGE_BYTE));
}

// Unreserve multiple pages
void unreserve_pages(void *address, uint64_t pageCount) {
    for(uint64_t i = 0; i < pageCount; i++)
        unreserve_page((void*)((uint64_t)address + i * PAGE_BYTE));
}

// Reserve multiple pages
void reserve_pages(void *address, uint64_t pageCount) {
    for(uint64_t i = 0; i < pageCount; i++)
        reserve_page((void*)((uint64_t)address + i * PAGE_BYTE));
}

// Request an unused page and lock it
void *request_page(RENDERER *R) {
    for(uint64_t i = 0; i < pageTritmap.size * TRYTE_TRIT; i++) {
        if(read_trit(pageTritmap.buffer, i) != tFALSE) continue;
        lock_page((void*)(i * PAGE_BYTE));
        if(PAGE_TRYTE > freeMemory) print(R, uint64_to_string(freeMemory));
        if(PAGE_TRYTE > freeMemory) print(R, " ");
        return (void*)(i * PAGE_BYTE);
    }
    return NULL;
}

// Read memory map
void read_efi_memory_map(EFI_MEMORY_DESCRIPTOR *map, size_t mapSize, size_t mapDescriptorSize) {
    if(initialized) return;
    initialized = true;
    
    uint64_t mapEntries = mapSize / mapDescriptorSize;
    void *largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;
    
    for(uint64_t i = 0; i < mapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + (i * mapDescriptorSize));
        if(desc->type == 7 && desc->pageCount > largestFreeMemSegSize) { // type = EfiConventionalMemory
            largestFreeMemSeg = desc->physicalAddress;
            largestFreeMemSegSize = desc->pageCount;
        }
    }

    uint64_t memorySize = get_memory_size(map, mapEntries, mapDescriptorSize);
    freeMemory = memorySize;

    init_page_tritmap(memorySize / PAGE_BYTE, largestFreeMemSeg);

    lock_pages(pageTritmap.buffer, ceil(pageTritmap.size, PAGE_TRYTE));
    
    for(uint32_t i = 0; i < mapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + (i * mapDescriptorSize));
        if(desc->type != 7) // type = EfiConventionalMemory
            reserve_pages(desc->physicalAddress, desc->pageCount);
    }
}

// Free RAM in trytes
uint64_t get_free_RAM() {
    return freeMemory;   
}

// Used RAM in trytes
uint64_t get_used_RAM() {
    return usedMemory;
}

// Reserved RAM in trytes
uint64_t get_reserved_RAM() {
    return reservedMemory; 
}