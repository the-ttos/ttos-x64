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

typedef struct {
    PAGE_TABLE *pml4;
} PAGE_TABLE_MANAGER;

void init_page_table_manager(PAGE_TABLE_MANAGER *manager, PAGE_TABLE *address) {
    manager->pml4 = address;
}

void map_memory(RENDERER *R, PAGE_TABLE_MANAGER *manager, void *virtualMemory, void *physicalMemory) {
    PAGE_MAP_INDEXER indexer;
    init_page_map_indexer(&indexer, (uint64_t)virtualMemory);
    PAGE_DIRECTORY_ENTRY pde;

    pde = manager->pml4->entries[indexer.PDP_i];
    PAGE_TABLE *pdp;
    if(!pde.present) {
        pdp = (PAGE_TABLE*)request_page(R);
        memset((uint8_t*)pdp, tryteEMPTY, PAGE_TRYTE);
        pde.address = (uint64_t)pdp >> 12;
        pde.present = true;
        pde.readWrite = true;
        manager->pml4->entries[indexer.PDP_i] = pde;
    } else pdp = (PAGE_TABLE*)((uint64_t)pde.address << 12);

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

    pde = pd->entries[indexer.PT_i];
    PAGE_TABLE *pt;
    if(!pde.present) {
        pt = (PAGE_TABLE*)request_page(R);
        memset((uint8_t*)pt, tryteEMPTY, PAGE_TRYTE);
        pde.address = (uint64_t)pt >> 12;
        pde.present = true;
        pde.readWrite = true;
        pd->entries[indexer.PT_i] = pde;
    } else pt = (PAGE_TABLE*)((uint64_t)pde.address << 12);
    
    // pde = pt->entries[indexer.P_i];
    // pde.address = (uint64_t)physicalMemory >> 12;
    // pde.present = true;
    // pde.readWrite = true;
    // pt->entries[indexer.P_i] = pde;
}