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
#include "pageframeallocator.h"
#endif

#define COLOR_OFFSET 0x00030200

typedef struct {
	FRAMEBUFFER *framebuffer;
	PSF1_FONT *font;
	EFI_MEMORY_DESCRIPTOR *map;
	uint64_t mapSize;
	uint64_t mapDescriptorSize;
} BOOT_INFO;

void _start(BOOT_INFO *bootInfo){
    RENDERER r = {bootInfo->framebuffer, ANCHOR, bootInfo->font, 0xff0020ff};

    for(uint16_t i = 0; i < bootInfo->framebuffer->width * 4; i++)
        for(uint16_t j = 0; j < bootInfo->framebuffer->height; j++)
            *(uint8_t*)(i + (j * bootInfo->framebuffer->pixelsPerScanline * 4) + bootInfo->framebuffer->address) = 0x00000000;
    
    uint64_t mapEntries = bootInfo->mapSize / bootInfo->mapDescriptorSize;

    read_efi_memory_map(bootInfo->map, bootInfo->mapSize, bootInfo->mapDescriptorSize);
    print(&r, "Free RAM: ");
    print(&r, tryte_to_string(get_free_RAM() / 2187));
    print(&r, " KT\n");
    print(&r, "Used RAM: ");
    print(&r, tryte_to_string(get_used_RAM() / 2187));
    print(&r, " KT\n");
    print(&r, "Reserved RAM: ");
    print(&r, tryte_to_string(get_reserved_RAM() / 2187));
    print(&r, " KT\n");


    // print(&r, uint64_to_string(get_memory_size(bootInfo->map, mapEntries, bootInfo->mapDescriptorSize)));

    // for(uint64_t i = 0; i < mapEntries; i++) {
    //     EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)bootInfo->map + (i * bootInfo->mapDescriptorSize));
    //     print(&r, efiMemoryTypeStrings[desc->type]);
    //     print(&r, " ");
    //     r.color = 0xffff00ff;
    //     print(&r, uint64_to_string(desc->pageCount * 4096 / 1024));
    //     print(&r, " ");
    //     print(&r, "KB");
    //     print(&r, "\n");
    //     r.color = 0xff0020ff;
    // }  

    /*
    print(&r, "\n==================== TRITMAP TESTS ====================\n");
    __tryte_buffer(b, 2) = { 0 };
    write_trit(b, 7, TRUE);
    write_trit(b, 8, UNKNOWN);
    write_trit(b, 9, FALSE);
    write_trit(b, 10, UNKNOWN);
    write_trit(b, 11, UNKNOWN);
    write_trit(b, 12, TRUE);
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
    r.color += COLOR_OFFSET;
    print(&r, "NOT t (27): ");
    print(&r, tryte_to_hstring(__not(t)));
    print(&r, "\n");
    print(&r, "NOT t (10): ");
    print(&r, tryte_to_string(__not(t)));
    print(&r, "\n");
    print(&r, "NOT t  (3): ");
    print(&r, tryte_to_tstring(__not(t)));
    r.color += COLOR_OFFSET;
    print(&r, "\n");
    print(&r, "INC t (27): ");
    print(&r, tryte_to_hstring(__inc(t)));
    print(&r, "\n");
    print(&r, "INC t (10): ");
    print(&r, tryte_to_string(__inc(t)));
    print(&r, "\n");
    print(&r, "INC t  (3): ");
    print(&r, tryte_to_tstring(__inc(t)));
    r.color += COLOR_OFFSET;
    print(&r, "\n");
    print(&r, "DEC t (27): ");
    print(&r, tryte_to_hstring(__dec(t)));
    print(&r, "\n");
    print(&r, "DEC t (10): ");
    print(&r, tryte_to_string(__dec(t)));
    print(&r, "\n");
    print(&r, "DEC t  (3): ");
    print(&r, tryte_to_tstring(__dec(t)));
    r.color += COLOR_OFFSET;
    print(&r, "\n");
    print(&r, "ISF t (27): ");
    print(&r, tryte_to_hstring(__isf(t)));
    print(&r, "\n");
    print(&r, "ISF t (10): ");
    print(&r, tryte_to_string(__isf(t)));
    print(&r, "\n");
    print(&r, "ISF t  (3): ");
    print(&r, tryte_to_tstring(__isf(t)));
    r.color += COLOR_OFFSET;
    print(&r, "\n");
    print(&r, "ISU t (27): ");
    print(&r, tryte_to_hstring(__isu(t)));
    print(&r, "\n");
    print(&r, "ISU t (10): ");
    print(&r, tryte_to_string(__isu(t)));
    print(&r, "\n");
    print(&r, "ISU t  (3): ");
    print(&r, tryte_to_tstring(__isu(t)));
    r.color += COLOR_OFFSET;
    print(&r, "\n");
    print(&r, "IST t (27): ");
    print(&r, tryte_to_hstring(__ist(t)));
    print(&r, "\n");
    print(&r, "IST t (10): ");
    print(&r, tryte_to_string(__ist(t)));
    print(&r, "\n");
    print(&r, "IST t  (3): ");
    print(&r, tryte_to_tstring(__ist(t)));
    r.color += COLOR_OFFSET;
    print(&r, "\n");
    print(&r, "CLD t (27): ");
    print(&r, tryte_to_hstring(__cld(t)));
    print(&r, "\n");
    print(&r, "CLD t (10): ");
    print(&r, tryte_to_string(__cld(t)));
    print(&r, "\n");
    print(&r, "CLD t  (3): ");
    print(&r, tryte_to_tstring(__cld(t)));
    r.color += COLOR_OFFSET;
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
    r.color += COLOR_OFFSET;
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
    r.color += COLOR_OFFSET;
    print(&r, "ID  v (27): ");
    print(&r, tryte_to_hstring(v));
    print(&r, "\n");
    print(&r, "ID  v (10): ");
    print(&r, tryte_to_string(v));
    print(&r, "\n");
    print(&r, "ID  v  (3): ");
    print(&r, tryte_to_tstring(v));
    print(&r, "\n");
    r.color += COLOR_OFFSET;
    print(&r, "AND u, v (27): ");
    print(&r, tryte_to_hstring(__and(u, v)));
    print(&r, "\n");
    print(&r, "AND u, v (10): ");
    print(&r, tryte_to_string(__and(u, v)));
    print(&r, "\n");
    print(&r, "AND u, v  (3): ");
    print(&r, tryte_to_tstring(__and(u, v)));
    print(&r, "\n");
    r.color += COLOR_OFFSET;
    print(&r, "OR  u, v (27): ");
    print(&r, tryte_to_hstring(__or(u, v)));
    print(&r, "\n");
    print(&r, "OR  u, v (10): ");
    print(&r, tryte_to_string(__or(u, v)));
    print(&r, "\n");
    print(&r, "OR  u, v  (3): ");
    print(&r, tryte_to_tstring(__or(u, v)));
    print(&r, "\n");
    r.color += COLOR_OFFSET;
    print(&r, "NAND  u, v (27): ");
    print(&r, tryte_to_hstring(__nand(u, v)));
    print(&r, "\n");
    print(&r, "NAND  u, v (10): ");
    print(&r, tryte_to_string(__nand(u, v)));
    print(&r, "\n");
    print(&r, "NAND  u, v  (3): ");
    print(&r, tryte_to_tstring(__nand(u, v)));
    print(&r, "\n");
    r.color += COLOR_OFFSET;
    print(&r, "NOR  u, v (27): ");
    print(&r, tryte_to_hstring(__nor(u, v)));
    print(&r, "\n");
    print(&r, "NOR  u, v (10): ");
    print(&r, tryte_to_string(__nor(u, v)));
    print(&r, "\n");
    print(&r, "NOR  u, v  (3): ");
    print(&r, tryte_to_tstring(__nor(u, v)));
    print(&r, "\n");
    r.color += COLOR_OFFSET;
    print(&r, "XOR  u, v (27): ");
    print(&r, tryte_to_hstring(__xor(u, v)));
    print(&r, "\n");
    print(&r, "XOR  u, v (10): ");
    print(&r, tryte_to_string(__xor(u, v)));
    print(&r, "\n");
    print(&r, "XOR  u, v  (3): ");
    print(&r, tryte_to_tstring(__xor(u, v)));
    print(&r, "\n");
    r.color += COLOR_OFFSET;
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