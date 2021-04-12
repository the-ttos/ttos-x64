#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#define IDT_TA_INTERRUPT_GATE   0b10001110
#define IDT_TA_CALL_GATE        0b10001100  
#define IDT_TA_TRAP_GATE        0b10001111

typedef struct {
    uint16_t offset0;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset1;
    uint32_t offset2;
} IDT_DESC_ENTRY;

typedef struct {
    uint16_t limit;
    uint64_t offset;
}__attribute__((packed)) IDTR;

void set_offset(IDT_DESC_ENTRY *idtDescEntry, uint64_t offset) {
    idtDescEntry->offset0 = (uint16_t)(offset & 0x000000000000ffff);
    idtDescEntry->offset1 = (uint16_t)((offset & 0x00000000ffff0000) >> 16);
    idtDescEntry->offset2 = (uint32_t)((offset & 0xffffffff00000000) >> 32);
}

uint64_t get_offset(IDT_DESC_ENTRY *idtDescEntry) {
    uint16_t offset = 0;
    offset |= (uint64_t)idtDescEntry->offset0;
    offset |= (uint64_t)idtDescEntry->offset1 << 16;
    offset |= (uint64_t)idtDescEntry->offset1 << 32;
    return offset;
}