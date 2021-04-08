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

extern uint64_t _kernelStart;
extern uint64_t _kernelEnd;

extern void _start(BOOT_INFO *b){
    bootInfo = b;
    RENDERER r = {bootInfo->framebuffer, ANCHOR, bootInfo->font, 0xfffbc531};

    for(uint16_t i = 0; i < bootInfo->framebuffer->width * 4; i++)
        for(uint16_t j = 0; j < bootInfo->framebuffer->height; j++)
            *(uint8_t*)(i + (j * bootInfo->framebuffer->pixelsPerScanline * 4) + bootInfo->framebuffer->address) = 0x00000000;

    uint64_t mapEntries = bootInfo->mapSize / bootInfo->mapDescriptorSize;

    read_efi_memory_map(bootInfo->map, bootInfo->mapSize, bootInfo->mapDescriptorSize);

    uint64_t kernelSize = (uint64_t)&_kernelEnd - (uint64_t)&_kernelStart;
    uint64_t kernelPages = ceil(kernelSize, PAGE_BYTE);

    lock_pages(&_kernelStart, kernelPages);
    
    PAGE_TABLE *pml4 = (PAGE_TABLE*)request_page(&r);
    BINARY_memset((uint8_t*)pml4, 0, PAGE_BYTE);
    // memset((uint8_t*)pml4, tryteEMPTY, PAGE_TRYTE);
    PAGE_TABLE_MANAGER pageTableManager;
    init_page_table_manager(&pageTableManager, pml4);

    // for(uint64_t t = 0;
    //     t < get_memory_size(bootInfo->map, mapEntries, bootInfo->mapDescriptorSize);
    //     t += PAGE_BYTE) {
    //     map_memory(&r, &pageTableManager, (void*)t, (void*)t);
    // }

    map_memory(&r, &pageTableManager, (void*)0, (void*)0);
    map_memory(&r, &pageTableManager, (void*)0, (void*)0);

    // uint64_t fbBase = (uint64_t)bootInfo->framebuffer->address;
    // uint64_t fbSize = (uint64_t)bootInfo->framebuffer->size + PAGE_BYTE;

    // for(uint64_t t = fbBase; t < fbBase + fbSize; t += PAGE_BYTE)
    //     map_memory(&r, &pageTableManager, (void*)t, (void*)t);

    // asm("mov %0, %%cr3" : : "r" (pml4));
    // print(&r, "Hello\n");

    // map_memory(&pageTableManager, (void*)0x600000000, (void*)0x80000);

    // uint64_t* test = (uint64_t*)0x600000000;
    // *test = 26;

    // print(&r, uint64_to_string(*test));

    print(&r, "Free RAM: ");
    print(&r, uint64_to_string(get_free_RAM() / 1024));
    print(&r, " KB\n");
    print(&r, "Used RAM: ");
    print(&r, uint64_to_string(get_used_RAM() / 1024));
    print(&r, " KB\n");
    print(&r, "Reserved RAM: ");
    print(&r, uint64_to_string(get_reserved_RAM() / 1024));
    print(&r, " KB\n");

    // print(&r, "Free RAM: ");
    // print(&r, uint64_to_string(get_free_RAM() / METRI));
    // print(&r, ".");
    // print(&r, uint64_to_string((get_free_RAM() * 100 / METRI) - (get_free_RAM() / METRI) * 100));
    // print(&r, " MT\n");
    // print(&r, "Used RAM: ");
    // print(&r, uint64_to_string(get_used_RAM() / KITRI));
    // print(&r, ".");
    // print(&r, uint64_to_string((get_used_RAM() * 100 / KITRI) - (get_used_RAM() / KITRI * 100)));
    // print(&r, " KT\n");
    // print(&r, "Reserved RAM: ");
    // print(&r, uint64_to_string(get_reserved_RAM() / METRI));
    // print(&r, ".");
    // print(&r, uint64_to_string((get_reserved_RAM() * 100 / METRI) - (get_reserved_RAM() / METRI * 100)));
    // print(&r, " MT\n");

    // __tryte(u) = {0b00000001, 0b01011010, 0b10000000};
    // __tryte(v) = {0b00011000, 0b01100001, 0b10000000};
    // memset((uint8_t*)0x12, __nand(u, v), 1);
    // print(&r, memview((uint8_t*)0x12, 3));

    /*
    print(&r, "\n==================== BITMAP TESTS ====================\n");
    uint8_t buf[2];
    write_bit(b, 7, true);
    write_bit(b, 8, false);
    write_bit(b, 9, true);
    write_bit(b, 10, false);
    write_bit(b, 11, false);
    write_bit(b, 12, true);
    for(uint8_t i = 7; i < 13; i++) {
        print(&r, uint64_to_string(read_bit(b, i)));
        print(&r, "\n");
    }

    print(&r, "\n==================== TRITMAP TESTS ====================\n");
    __tryte_buffer(b, 2) = { 0 };
    write_trit(b, 7, tTRUE);
    write_trit(b, 8, tUNKNOWN);
    write_trit(b, 9, tFALSE);
    write_trit(b, 10, tUNKNOWN);
    write_trit(b, 11, tUNKNOWN);
    write_trit(b, 12, tTRUE);
    for(uint8_t i = 7; i < 13; i++) {
        print(&r, trit_to_bstring(read_trit(b, i)));
        print(&r, "\n");
    }

    print(&r, "\n====================  GATE TESTS  =====================\n");
    __tryte(t) = {0b10010010, 0b10001001, 0b00000000};
    print(&r, "ID  t (27): ");
    print(&r, tryte_to_hstring(t));
    print(&r, "\n");
    print(&r, "ID  t (10): ");
    print(&r, tryte_to_string(t));
    print(&r, "\n");
    print(&r, "ID  t  (3): ");
    print(&r, tryte_to_tstring(t));
    print(&r, "\n");
    print(&r, "NOT t (27): ");
    print(&r, tryte_to_hstring(__not(t)));
    print(&r, "\n");
    print(&r, "NOT t (10): ");
    print(&r, tryte_to_string(__not(t)));
    print(&r, "\n");
    print(&r, "NOT t  (3): ");
    print(&r, tryte_to_tstring(__not(t)));
    print(&r, "\n");
    print(&r, "INC t (27): ");
    print(&r, tryte_to_hstring(__inc(t)));
    print(&r, "\n");
    print(&r, "INC t (10): ");
    print(&r, tryte_to_string(__inc(t)));
    print(&r, "\n");
    print(&r, "INC t  (3): ");
    print(&r, tryte_to_tstring(__inc(t)));
    print(&r, "\n");
    print(&r, "DEC t (27): ");
    print(&r, tryte_to_hstring(__dec(t)));
    print(&r, "\n");
    print(&r, "DEC t (10): ");
    print(&r, tryte_to_string(__dec(t)));
    print(&r, "\n");
    print(&r, "DEC t  (3): ");
    print(&r, tryte_to_tstring(__dec(t)));
    print(&r, "\n");
    print(&r, "ISF t (27): ");
    print(&r, tryte_to_hstring(__isf(t)));
    print(&r, "\n");
    print(&r, "ISF t (10): ");
    print(&r, tryte_to_string(__isf(t)));
    print(&r, "\n");
    print(&r, "ISF t  (3): ");
    print(&r, tryte_to_tstring(__isf(t)));
    print(&r, "\n");
    print(&r, "ISU t (27): ");
    print(&r, tryte_to_hstring(__isu(t)));
    print(&r, "\n");
    print(&r, "ISU t (10): ");
    print(&r, tryte_to_string(__isu(t)));
    print(&r, "\n");
    print(&r, "ISU t  (3): ");
    print(&r, tryte_to_tstring(__isu(t)));
    print(&r, "\n");
    print(&r, "IST t (27): ");
    print(&r, tryte_to_hstring(__ist(t)));
    print(&r, "\n");
    print(&r, "IST t (10): ");
    print(&r, tryte_to_string(__ist(t)));
    print(&r, "\n");
    print(&r, "IST t  (3): ");
    print(&r, tryte_to_tstring(__ist(t)));
    print(&r, "\n");
    print(&r, "CLD t (27): ");
    print(&r, tryte_to_hstring(__cld(t)));
    print(&r, "\n");
    print(&r, "CLD t (10): ");
    print(&r, tryte_to_string(__cld(t)));
    print(&r, "\n");
    print(&r, "CLD t  (3): ");
    print(&r, tryte_to_tstring(__cld(t)));
    print(&r, "\n");
    print(&r, "CLU t (27): ");
    print(&r, tryte_to_hstring(__clu(t)));
    print(&r, "\n");
    print(&r, "CLU t (10): ");
    print(&r, tryte_to_string(__clu(t)));
    print(&r, "\n");
    print(&r, "CLU t  (3): ");
    print(&r, tryte_to_tstring(__clu(t)));
    print(&r, "\n");
    __tryte(u) = {0b00000001, 0b01011010, 0b10000000};
    print(&r, "ID  u (27): ");
    print(&r, tryte_to_hstring(u));
    print(&r, "\n");
    print(&r, "ID  u (10): ");
    print(&r, tryte_to_string(u));
    print(&r, "\n");
    print(&r, "ID  u  (3): ");
    print(&r, tryte_to_tstring(u));
    print(&r, "\n");
    __tryte(v) = {0b00011000, 0b01100001, 0b10000000};
    print(&r, "ID  v (27): ");
    print(&r, tryte_to_hstring(v));
    print(&r, "\n");
    print(&r, "ID  v (10): ");
    print(&r, tryte_to_string(v));
    print(&r, "\n");
    print(&r, "ID  v  (3): ");
    print(&r, tryte_to_tstring(v));
    print(&r, "\n");
    print(&r, "AND u, v (27): ");
    print(&r, tryte_to_hstring(__and(u, v)));
    print(&r, "\n");
    print(&r, "AND u, v (10): ");
    print(&r, tryte_to_string(__and(u, v)));
    print(&r, "\n");
    print(&r, "AND u, v  (3): ");
    print(&r, tryte_to_tstring(__and(u, v)));
    print(&r, "\n");
    print(&r, "OR  u, v (27): ");
    print(&r, tryte_to_hstring(__or(u, v)));
    print(&r, "\n");
    print(&r, "OR  u, v (10): ");
    print(&r, tryte_to_string(__or(u, v)));
    print(&r, "\n");
    print(&r, "OR  u, v  (3): ");
    print(&r, tryte_to_tstring(__or(u, v)));
    print(&r, "\n");
    print(&r, "NAND  u, v (27): ");
    print(&r, tryte_to_hstring(__nand(u, v)));
    print(&r, "\n");
    print(&r, "NAND  u, v (10): ");
    print(&r, tryte_to_string(__nand(u, v)));
    print(&r, "\n");
    print(&r, "NAND  u, v  (3): ");
    print(&r, tryte_to_tstring(__nand(u, v)));
    print(&r, "\n");
    print(&r, "NOR  u, v (27): ");
    print(&r, tryte_to_hstring(__nor(u, v)));
    print(&r, "\n");
    print(&r, "NOR  u, v (10): ");
    print(&r, tryte_to_string(__nor(u, v)));
    print(&r, "\n");
    print(&r, "NOR  u, v  (3): ");
    print(&r, tryte_to_tstring(__nor(u, v)));
    print(&r, "\n");
    print(&r, "XOR  u, v (27): ");
    print(&r, tryte_to_hstring(__xor(u, v)));
    print(&r, "\n");
    print(&r, "XOR  u, v (10): ");
    print(&r, tryte_to_string(__xor(u, v)));
    print(&r, "\n");
    print(&r, "XOR  u, v  (3): ");
    print(&r, tryte_to_tstring(__xor(u, v)));
    print(&r, "\n");
    print(&r, "SUM  u, v (27): ");
    print(&r, tryte_to_hstring(__sum(u, v)));
    print(&r, "\n");
    print(&r, "SUM  u, v (10): ");
    print(&r, tryte_to_string(__sum(u, v)));
    print(&r, "\n");
    print(&r, "SUM  u, v  (3): ");
    print(&r, tryte_to_tstring(__sum(u, v)));
    print(&r, "\n");
    */
}