#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
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

PAGE_TABLE *pml4;

void init_page_table_manager(PAGE_TABLE *address) {
    pml4 = address;
}

void map_memory(void *virtualMemory, void *physicalMemory) {
    PAGE_MAP_INDEXER indexer = new_page_map_indexer((uint64_t)virtualMemory);
    PAGE_DIRECTORY_ENTRY entry;

    entry = pml4->entries[indexer.PDP_i];
    PAGE_TABLE *PDP;
    if(!entry.present) PDP = (PAGE_TABLE*)request_page();
}