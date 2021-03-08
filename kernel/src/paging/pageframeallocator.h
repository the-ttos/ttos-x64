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
BITMAP pageBitmap;
TRITMAP pageTritmap;

void init_page_bitmap(size_t bytes, void *bufferAddress) {
    pageBitmap.size = bytes;
    pageBitmap.buffer = (uint8_t*)bufferAddress;
    for(size_t i = 0; i < pageBitmap.size; i++) *(uint8_t*)(pageBitmap.buffer + i) = 0;
}

// Initialize the pages Tritmap
void init_page_tritmap(size_t trytes, void *bufferAddress) {
    pageTritmap.size = trytes;
    pageTritmap.buffer = (uint8_t*)bufferAddress;
    for(size_t i = 0; i < pageTritmap.size; i++) *(uint8_t*)(pageTritmap.buffer + i) = 0;
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

void BINARY_free_page(void *address) {
    uint64_t index = (uint64_t)address / PAGE_BYTE;
    if(!read_bit(pageBitmap.buffer, index)) return;
    write_bit(pageBitmap.buffer, index, false);
    freeMemory += PAGE_BYTE;
    usedMemory -= PAGE_BYTE;
}

void BINARY_lock_page(void *address) {
    uint64_t index = (uint64_t)address / PAGE_BYTE;
    if(read_bit(pageBitmap.buffer, index)) return;
    write_bit(pageBitmap.buffer, index, true);
    freeMemory -= PAGE_BYTE;
    usedMemory += PAGE_BYTE;
}

void BINARY_unreserve_page(void *address) {
    uint64_t index = (uint64_t)address / PAGE_BYTE;
    if(!read_bit(pageBitmap.buffer, index)) return;
    write_bit(pageBitmap.buffer, index, false);
    freeMemory += PAGE_BYTE;
    reservedMemory -= PAGE_BYTE;
}

void BINARY_reserve_page(RENDERER *R, void *address) {
    uint64_t index = (uint64_t)address / PAGE_BYTE;
    print(R, uint64_to_string(index));
    print(R, "\n");
    if(read_bit(pageBitmap.buffer, index)) return;
    write_bit(pageBitmap.buffer, index, true);
    freeMemory -= PAGE_BYTE;
    reservedMemory += PAGE_BYTE;
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
        print(R, "asd");
        if(read_trit(pageTritmap.buffer, i) != tFALSE) continue;
        lock_page((void*)(i * PAGE_BYTE));
        return (void*)(i * PAGE_BYTE);
    }
    return NULL;
}

void *BINARY_request_page() {
    for(uint64_t i = 0; i < pageBitmap.size * CHAR_BIT; i++) {
        if(read_bit(pageBitmap.buffer, i)) continue;
        BINARY_lock_page((void*)(i * PAGE_BYTE));
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
        if(desc->type == 7 && desc->pageCount * PAGE_TRYTE > largestFreeMemSegSize) { // type = EfiConventionalMemory
            largestFreeMemSeg = desc->physicalAddress;
            largestFreeMemSegSize = desc->pageCount * PAGE_TRYTE;
        }
    }

    uint64_t memorySize = get_memory_size(map, mapEntries, mapDescriptorSize);
    freeMemory = memorySize;

    uint64_t tritmapSize = ceil(memorySize / PAGE_TRYTE, CHAR_BIT);
    init_page_tritmap(tritmapSize, largestFreeMemSeg);

    lock_pages(pageTritmap.buffer, ceil(pageBitmap.size, PAGE_TRYTE));
    
    for(uint32_t i = 0; i < mapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + (i * mapDescriptorSize));
        if(desc->type != 7) // type = EfiConventionalMemory
            reserve_pages(desc->physicalAddress, desc->pageCount);
    }
}

void BINARY_read_efi_memory_map(EFI_MEMORY_DESCRIPTOR *map, size_t mapSize, size_t mapDescriptorSize) {
    if(initialized) return;
    initialized = true;
    
    uint64_t mapEntries = mapSize / mapDescriptorSize;
    void *largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;
    
    for(uint64_t i = 0; i < mapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + (i * mapDescriptorSize));
        if(desc->type == 7 && desc->pageCount * PAGE_BYTE > largestFreeMemSegSize) { // type = EfiConventionalMemory
            largestFreeMemSeg = desc->physicalAddress;
            largestFreeMemSegSize = desc->pageCount * PAGE_BYTE;
        }
    }

    uint64_t memorySize = BINARY_get_memory_size(map, mapEntries, mapDescriptorSize);
    freeMemory = memorySize;

    uint64_t bitmapSize = memorySize / PAGE_BYTE / CHAR_BIT + 1;
    init_page_bitmap(bitmapSize, largestFreeMemSeg);

    lock_pages(pageBitmap.buffer, pageBitmap.size / PAGE_BYTE + 1);
    
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