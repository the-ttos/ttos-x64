#ifndef PANIC_H
#define PANIC_H
#include "../panic.h"
#endif

typedef struct {} INTERRUPT_FRAME;

__attribute__((interrupt)) void page_fault_handler(INTERRUPT_FRAME *frame) {
    panic("Page fault detected.");
    while(true);
}