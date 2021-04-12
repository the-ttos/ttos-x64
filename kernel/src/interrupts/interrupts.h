typedef struct {

} INTERRUPT_FRAME;

__attribute__((interrupt)) void page_fault_handler(INTERRUPT_FRAME *frame) {
    
    while(true);
}