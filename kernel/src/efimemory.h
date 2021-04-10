#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

// EFI Memory Descriptor structure
typedef struct {
    uint32_t type;
    void *physicalAddress;
    void *virtualAddress;
    uint64_t pageCount;
    uint64_t attributes;
} EFI_MEMORY_DESCRIPTOR;

// EFI Memory Type strings
const char *efiMemoryTypeStrings[] = {
    "EfiReservedMemoryType",
    "EfiLoaderCode",
    "EfiLoaderData",
    "EfiBootServicesCode",
    "EfiBootServicesData",
    "EfiRuntimeServicesCode",
    "EfiRuntimeServicesData",
    "EfiConventionalMemory",
    "EfiUnusableMemory",
    "EfiACPIReclaimMemory",
    "EfiACPIMemoryNVS",
    "EfiMemoryMappedIO",
    "EfiMemoryMappedIOPortSpace",
    "EfiPalCode"
};

