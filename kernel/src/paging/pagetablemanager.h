#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

#ifndef PAGING_H
#define PAGING_H
#include "paging.h"
#endif

#ifndef PAGEMAPINDEXER_H
#define PAGEMAPINDEXER_H
#include "pagemapindexer.h"
#endif

#ifndef PAGEFRAMEALLOCATOR_H
#define PAGEFRAMEALLOCATOR_H
#include "pageframeallocator.h"
#endif

// Page Table Manager structure
typedef struct {
    PAGE_TABLE *pml4;
} PAGE_TABLE_MANAGER;

// Page Table Manager constructor
void init_page_table_manager(PAGE_TABLE_MANAGER *manager, PAGE_TABLE *address) {
    manager->pml4 = address;
}

// Map memory in bytes
// =============================================================
// Memory is not directly virtualized into ternary units because
// 9 (trits) is not divisible by 8 (bits). This difference
// causes an offset to happen and it's only normalized every 4 
// bytes, which means that the only direct way of virtualizing
// trytes would be mapping the memory in a 4:1 bytes proportion.
// 
// However, this would cause necessity of another conversion
// process, since 4 bytes is equivalent to 3 trytes, therefore
// non atomic. Instead, a binary PML4 structure is still used,
// but tryte-alignment is enforced when writing or reading
// trytes from memory.
// =============================================================
void map_memory(RENDERER *R, PAGE_TABLE_MANAGER *manager, void *virtualMemory, void *physicalMemory) {
    PAGE_MAP_INDEXER indexer;
    init_page_map_indexer(&indexer, (uint64_t)virtualMemory);
    PAGE_DIRECTORY_ENTRY pde;

    // PML4
    pde = manager->pml4->entries[indexer.PDP_i];
    PAGE_TABLE *pdp;
    if(!pde.present) {
        pdp = (PAGE_TABLE*)request_page(R);
        memset((uint8_t*)pdp, tryteEMPTY, PAGE_TRYTE);
        pde.address = (uint64_t)pdp >> 12;
        pde.present = true;
        pde.readWrite = true;
        manager->pml4->entries[indexer.PDP_i] = pde;
    } else { 
        pdp = (PAGE_TABLE*)((uint64_t)pde.address << 12); // 🤔
    }

    // PML3
    pde = pdp->entries[indexer.PD_i];
    PAGE_TABLE *pd;
    if(!pde.present) {
        pd = (PAGE_TABLE*)request_page(R);
        memset((uint8_t*)pd, tryteEMPTY, PAGE_TRYTE);
        pde.address = (uint64_t)pd >> 12;
        pde.present = true;
        pde.readWrite = true;
        pdp->entries[indexer.PD_i] = pde;
    } else pd = (PAGE_TABLE*)((uint64_t)pde.address << 12);

    // PML2
    pde = pd->entries[indexer.PT_i];
    PAGE_TABLE *pt;
    if(!pde.present) {
        pt = (PAGE_TABLE*)request_page(R);
        memset((uint8_t*)pt, tryteEMPTY, PAGE_TRYTE);
        pde.address = (uint64_t)pt >> 12;
        pde.present = true;
        pde.readWrite = true;
        pd->entries[indexer.PT_i] = pde;
    } else {
        pt = (PAGE_TABLE*)((uint64_t)pde.address << 12);
    }

    // PML1
    pde = pt->entries[indexer.P_i];
    pde.address = (uint64_t)physicalMemory >> 12;
    pde.present = true;
    pde.readWrite = true;
    pt->entries[indexer.P_i] = pde; 
}