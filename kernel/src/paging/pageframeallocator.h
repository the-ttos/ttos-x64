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

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
uint8_t initialized = 0;
BITMAP pageBitmap;

void init_page_bitmap(size_t bytes, void *bufferAddress) {
    pageBitmap.size = bytes;
    pageBitmap.buffer = (uint8_t*)bufferAddress;
    for(size_t i = 0; i < pageBitmap.size; i++) *(uint8_t*)(pageBitmap.buffer + i) = 0;
}

void free_page(void *address) {
    uint64_t index = (uint64_t)address / PAGE_BYTE;
    if(!read_bit(pageBitmap.buffer, index)) return;
    write_bit(pageBitmap.buffer, index, false);
    freeMemory += PAGE_TRYTE;
    usedMemory -= PAGE_TRYTE;
}

void lock_page(void *address) {
    uint64_t index = (uint64_t)address / PAGE_BYTE;
    if(read_bit(pageBitmap.buffer, index)) return;
    write_bit(pageBitmap.buffer, index, true);
    freeMemory -= PAGE_TRYTE;
    usedMemory += PAGE_TRYTE;
}

void unreserve_page(void *address) {
    uint64_t index = (uint64_t)address / PAGE_BYTE;
    if(!read_bit(pageBitmap.buffer, index)) return;
    write_bit(pageBitmap.buffer, index, false);
    freeMemory += PAGE_TRYTE;
    reservedMemory -= PAGE_TRYTE;
}

void reserve_page(void *address) {
    uint64_t index = (uint64_t)address / PAGE_BYTE;
    if(read_bit(pageBitmap.buffer, index)) return;
    write_bit(pageBitmap.buffer, index, true);
    freeMemory -= PAGE_TRYTE;
    reservedMemory += PAGE_TRYTE;
}

void free_pages(void *address, uint64_t pageCount) {
    for(uint64_t i = 0; i < pageCount; i++)
        free_page((void*)((uint64_t)address + (i * PAGE_BYTE)));
}

void lock_pages(void *address, uint64_t pageCount) {
    for(uint64_t i = 0; i < pageCount; i++)
        lock_page((void*)((uint64_t)address + (i * PAGE_BYTE)));
}

void unreserve_pages(void *address, uint64_t pageCount) {
    for(uint64_t i = 0; i < pageCount; i++)
        unreserve_page((void*)((uint64_t)address + (i * PAGE_BYTE)));
}

void reserve_pages(void *address, uint64_t pageCount) {
    for(uint64_t i = 0; i < pageCount; i++)
        reserve_page((void*)((uint64_t)address + (i * PAGE_BYTE)));
}

void *request_page() {
    for(uint64_t i = 0; i < pageBitmap.size * PAGE_TRYTE; i++) {
        if(read_bit(pageBitmap.buffer, i)) continue;
        lock_page((void*)(i * PAGE_BYTE));
        return (void*)(i * PAGE_BYTE);
    }
    return NULL;
}

void read_efi_memory_map(EFI_MEMORY_DESCRIPTOR *map, size_t mapSize, size_t mapDescriptorSize) {
    if(initialized) return;
    initialized = 1;
    
    uint64_t mapEntries = mapSize / mapDescriptorSize;
    void *largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;
    
    for(uint32_t i = 0; i < mapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)map + (i * mapDescriptorSize));
        if(desc->type == 7 && desc->pageCount * PAGE_TRYTE > largestFreeMemSegSize) { // type = EfiConventionalMemory
            largestFreeMemSeg = desc->physicalAddress;
            largestFreeMemSegSize = desc->pageCount * PAGE_TRYTE;
        }
    }

    uint64_t memorySize = get_memory_size(map, mapEntries, mapDescriptorSize);
    freeMemory = memorySize;

    uint64_t bitmapSize = ceil(memorySize / PAGE_TRYTE, CHAR_BIT);
    init_page_bitmap(bitmapSize, largestFreeMemSeg);

    lock_pages(pageBitmap.buffer, ceil(pageBitmap.size / PAGE_TRYTE, CHAR_BIT));
    
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