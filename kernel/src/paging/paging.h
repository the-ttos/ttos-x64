#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

// Page Directory Entry Enum
typedef enum {
    PDE_PRESENT = 0,
    PDE_READ_WRITE,
    PDE_USER_SUPER,
    PDE_WRITE_THROUGH,
    PDE_CACHE_DISABLED,
    PDE_ACCESSED,
    PDE_LARGER_PAGES = 7,
    PDE_CUSTOM0 = 9,
    PDE_CUSTOM1,
    PDE_CUSTOM2,
    PDE_CUSTOM3,
    PDE_NX = 63
} PAGE_DIRECTORY_ENTRY_ENUM;

// Page Directory Entry structure (MMU)
typedef struct {
    uint64_t value;
} PAGE_DIRECTORY_ENTRY;

// Page Table structure
typedef struct {
    PAGE_DIRECTORY_ENTRY entries[512];
} __attribute__ ((aligned (PAGE_BYTE))) PAGE_TABLE;

// Set flag on Page Directory Entry
void set_flag(PAGE_DIRECTORY_ENTRY *pageDirectoryEntry, PAGE_DIRECTORY_ENTRY_ENUM flag, bool enabled) {
    if(enabled) pageDirectoryEntry->value |= (uint64_t)1 << flag;
    else pageDirectoryEntry->value &= ~((uint64_t)1 << flag);
}

// Get flag on Page Directory Entry
bool get_flag(PAGE_DIRECTORY_ENTRY *pageDirectoryEntry, PAGE_DIRECTORY_ENTRY_ENUM flag) {
    return pageDirectoryEntry->value & (uint64_t)1 << flag > 0;
}

// Get address of Page Directory Entry
uint64_t get_address(PAGE_DIRECTORY_ENTRY *pageDirectoryEntry) {
    return (pageDirectoryEntry->value & 0x000ffffffffff000) >> 12;
}

// Set address of Page Directory Entry
void set_address(PAGE_DIRECTORY_ENTRY *pageDirectoryEntry, uint64_t address) {
    pageDirectoryEntry->value &= 0xfff0000000000fff;
    pageDirectoryEntry->value |= (address & 0x000000ffffffffff) << 12;
}