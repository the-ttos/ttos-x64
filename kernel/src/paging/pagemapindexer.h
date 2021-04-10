#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

// Page Map Indexer structure
typedef struct {
    uint64_t PDP_i;
    uint64_t PD_i;
    uint64_t PT_i;
    uint64_t P_i;
} PAGE_MAP_INDEXER;

// Page Map Indexer constructor
void init_page_map_indexer(PAGE_MAP_INDEXER *indexer, uint64_t virtualAddress) {
    virtualAddress >>= 12;
    indexer->P_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    indexer->PT_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    indexer->PD_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    indexer->PDP_i = virtualAddress & 0x1ff;
}