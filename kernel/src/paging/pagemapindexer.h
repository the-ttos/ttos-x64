#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

typedef struct {
    uint64_t PDP_i;
    uint64_t PD_i;
    uint64_t PT_i;
    uint64_t P_i;
} PAGE_MAP_INDEXER;

PAGE_MAP_INDEXER new_page_map_indexer(uint64_t virtualAddress) {
    PAGE_MAP_INDEXER indexer;   
    virtualAddress >>= 12;
    indexer.P_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    indexer.PT_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    indexer.PD_i = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    indexer.PDP_i = virtualAddress & 0x1ff;
    return indexer;
}