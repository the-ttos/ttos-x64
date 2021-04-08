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
        print(R, "PDE from pml4 DOESNT EXIST");
        print(R, "\n");
        pdp = (PAGE_TABLE*)request_page(R);
        BINARY_memset((uint8_t*)pdp, 0, PAGE_BYTE);
        // memset((uint8_t*)pdp, tryteEMPTY, PAGE_TRYTE);
        print(R, "new page address: ");
        print(R, uint64_to_string((uint64_t)pdp));
        print(R, "\n");
        pde.address = (uint64_t)pdp >> 12;
        print(R, "pde address: ");
        print(R, uint64_to_string((uint64_t)pdp >> 12));
        print(R, "\n");
        pde.present = true;
        pde.readWrite = true;
        manager->pml4->entries[indexer.PDP_i] = pde;
        print(R, "pml4 address (pdp entry): ");
        print(R, uint64_to_string((uint64_t)manager->pml4->entries[indexer.PDP_i].address));
        print(R, "\n");
    } else { 
        print(R, "PDE from pml4 EXISTS");
        print(R, "\n");
        print(R, "pde address: ");
        print(R, uint64_to_string((uint64_t)pde.address));
        print(R, "\n");
        pdp = (PAGE_TABLE*)((uint64_t)pde.address << 12);
        print(R, "pdp address: ");
        print(R, uint64_to_string((uint64_t)pdp));
        print(R, "\n");
    }

    pde = pdp->entries[indexer.PD_i];
    print(R, "pde address (pdp entry): ");
    print(R, uint64_to_string((uint64_t)pdp));
    print(R, "\n");
    PAGE_TABLE *pd;
    if(!pde.present) {
        print(R, "PDE from pdp DOESNT EXIST");
        pd = (PAGE_TABLE*)request_page(R);
        BINARY_memset((uint8_t*)pd, 0, PAGE_BYTE);
        // memset((uint8_t*)pd, tryteEMPTY, PAGE_TRYTE);
        pde.address = (uint64_t)pd >> 12;
        pde.present = true;
        pde.readWrite = true;
        print(R, "pdp address entry before pde: ");
        print(R, uint64_to_string((uint64_t)pdp->entries[indexer.PD_i].address));
        print(R, "\n");
        pdp->entries[indexer.PD_i] = pde;
        print(R, "pdp address entry after pde: ");
        print(R, uint64_to_string((uint64_t)pdp->entries[indexer.PD_i].address));
        print(R, "\n");
    } else {
        print(R, "PDE from pdp EXISTS");
        print(R, "\n");
        print(R, "pde address: ");
        print(R, uint64_to_string((uint64_t)pde.address));
        print(R, "\n");
        pd = (PAGE_TABLE*)((uint64_t)pde.address << 12);
        print(R, "pd: ");
        print(R, uint64_to_string((uint64_t)pd));
        print(R, "\n");
    }

    // Quando descomenta o último memset ternário, o pd->entries da iteração 513 falha
    // if((uint64_t)virtualMemory >= 512 * PAGE_BYTE) {
    //     print(R, uint64_to_string((uint64_t)virtualMemory / PAGE_BYTE));
    //     print(R, ": ");
    //     print(R, uint64_to_string((uint64_t)pd->entries[0].address));
    //     print(R, " ");
    // }

    pde = pd->entries[indexer.PT_i];
    PAGE_TABLE *pt;
    if(!pde.present) {
        print(R, "PDE from pt DOES NOT EXIST");
        print(R, "\n");
        pt = (PAGE_TABLE*)request_page(R);
        BINARY_memset((uint8_t*)pt, 0, PAGE_BYTE);
        // memset((uint8_t*)pt, tryteEMPTY, PAGE_TRYTE);
        pde.address = (uint64_t)pt >> 12;
        pde.present = true;
        pde.readWrite = true;
        pd->entries[indexer.PT_i] = pde;
    } else { 
        print(R, "PDE from pt EXISTS");
        print(R, "\n");
        print(R, "pde address: ");
        print(R, uint64_to_string((uint64_t)pde.address));
        print(R, "\n");
        pt = (PAGE_TABLE*)((uint64_t)pde.address << 12);
        print(R, "pt: ");
        print(R, uint64_to_string((uint64_t)pt));
        print(R, "\n");
    }

    pde = pt->entries[indexer.P_i];
    pde.address = (uint64_t)physicalMemory >> 12;
    pde.present = true;
    pde.readWrite = true;

    // print(R, uint64_to_string((uint64_t)manager->pml4->entries[indexer.PDP_i].address));
    // print(R, "\n");
    pt->entries[indexer.P_i] = pde; // Comentar essa linha faz funcionar
}