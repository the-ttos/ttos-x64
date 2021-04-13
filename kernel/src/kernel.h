#ifndef RENDERER_H
#define RENDERER_H
#include "renderer.h"
#endif

#ifndef CSTR_H
#define CSTR_H
#include "cstr.h"
#endif

#ifndef FONTS_H
#define FONTS_H
#include "fonts.h"
#endif

#ifndef TRYTE_H
#define TRYTE_H
#include "tryte.h"
#endif

#ifndef EFIMEMORY_H
#define EFIMEMORY_H
#include "efimemory.h"
#endif

#ifndef MEMORY_H
#define MEMORY_H
#include "memory.h"
#endif

#ifndef TRITMAP_H
#define TRITMAP_H
#include "tritmap.h"
#endif

#ifndef PAGEFRAMEALLOCATOR_H
#define PAGEFRAMEALLOCATOR_H
#include "paging/pageframeallocator.h"
#endif

#ifndef PAGETABLEMANAGER_H
#define PAGETABLEMANAGER_H
#include "paging/pagetablemanager.h"
#endif

#ifndef PAGING_H
#define PAGING_H
#include "paging/paging.h"
#endif

#ifndef PAGEMAPINDEXER_H
#define PAGEMAPINDEXER_H
#include "paging/pagemapindexer.h"
#endif

#ifndef BOOT_H
#define BOOT_H
#include "boot.h"
#endif

#ifndef GDT_H
#define GDT_H
#include "gdt/gdt.h"
#endif

#ifndef IDT_H
#define IDT_H
#include "interrupts/idt.h"
#endif

#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "interrupts/interrupts.h"
#endif


extern uint64_t _kernelStart;
extern uint64_t _kernelEnd;

typedef struct {
    PAGE_TABLE_MANAGER pageTableManager;
} KERNEL_INFO;

IDTR idtr;
void prepare_interrupts() {
    // Initialize IDTR
    idtr.limit = 0x0fff;
    idtr.offset = (uint64_t)request_page();

    IDT_DESC_ENTRY *int_pageFault = (IDT_DESC_ENTRY*)(idtr.offset + 0xe * sizeof(IDT_DESC_ENTRY));
    set_offset(int_pageFault, (uint64_t)page_fault_handler);
    int_pageFault->type_attr = IDT_TA_INTERRUPT_GATE;
    int_pageFault->selector = 0x08;

    asm("lidt %0" : : "m" (idtr));
}

PAGE_TABLE_MANAGER prepare_memory(BOOT_INFO *bootInfo) {
    // Get map descriptors and initialize tritmap
    uint64_t mapEntries = bootInfo->mapSize / bootInfo->mapDescriptorSize;
    read_efi_memory_map(bootInfo->map, bootInfo->mapSize, bootInfo->mapDescriptorSize);

    // Lock kernel pages
    uint64_t kernelSize = (uint64_t)&_kernelEnd - (uint64_t)&_kernelStart;
    uint64_t kernelPages = ceil(kernelSize, PAGE_BYTE);
    lock_pages(&_kernelStart, kernelPages);
    
    // Initialize pages structure
    // =============================================================
    //  Pages are guaranteed to be tryte-aligned because PAGE_BYTE is
    //  originally divisible by 4 (0x1000).
    // =============================================================
    PAGE_TABLE *pml4 = (PAGE_TABLE*)request_page();
    memset((uint8_t*)pml4, tryteEMPTY, PAGE_TRYTE);
    PAGE_TABLE_MANAGER pageTableManager = (PAGE_TABLE_MANAGER){.pml4 = pml4};

    // Map pages
    for(uint64_t t = 0;
        t < get_memory_size(bootInfo->map, mapEntries, bootInfo->mapDescriptorSize);
        t += PAGE_BYTE)
        map_memory(&pageTableManager, (void*)t, (void*)t);

    // Lock frame buffer pages
    uint64_t fbBase = (uint64_t)bootInfo->framebuffer->address;
    uint64_t fbSize = (uint64_t)bootInfo->framebuffer->size + PAGE_BYTE;
    lock_pages((void*)fbBase, ceil(fbSize, PAGE_BYTE));

    // Map frame buffer pages
    for(uint64_t t = fbBase; t < fbBase + fbSize; t += PAGE_BYTE)
        map_memory(&pageTableManager, (void*)t, (void*)t);

    // Apply mapping
    asm("mov %0, %%cr3" : : "r" (pml4));

    return pageTableManager;
}

KERNEL_INFO start_kernel(BOOT_INFO *bootInfo) {
    // Make GDT descriptor and load GDT
    GDT_DESCRIPTOR gdtDescriptor = {
        .size = sizeof(GDT) - 1,
        .offset = (uint64_t)&defaultGDT
    };
    LoadGDT(&gdtDescriptor);

    // Initialize memory
    KERNEL_INFO kernelInfo = {
        .pageTableManager = prepare_memory(bootInfo)
    };

    // Initialize interrupts
    prepare_interrupts();

    // Reset frame buffer
    // =============================================================
    //  Binary functions are used because frame buffer is not 
    //  guaranteed to be tryte-aligned.
    // =============================================================
    memset_BINARY(bootInfo->framebuffer->address, 0, bootInfo->framebuffer->size);

    return kernelInfo;
}