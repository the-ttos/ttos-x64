#ifndef KERNEL_H
#define KERNEL_H
#include "kernel.h"
#endif

/*

    d888888b d8888b. d888888b .d8888. d88888b  .o88b. d888888b 
    `~~88~~' 88  `8D   `88'   88'  YP 88'     d8P  Y8 `~~88~~' 
       88    88oobY'    88    `8bo.   88ooooo 8P         88    
       88    88`8b      88      `Y8b. 88~~~~~ 8b         88    
       88    88 `88.   .88.   db   8D 88.     Y8b  d8    88    
       YP    88   YD Y888888P `8888Y' Y88888P  `Y88P'    YP    

*/  

extern void _start(BOOT_INFO *bootInfo){
    KERNEL_INFO kernelInfo = start_kernel(bootInfo);
    
    // Renderer
    RENDERER r = {bootInfo->framebuffer, ANCHOR, bootInfo->font, 0xfffbc531};

    // Reset frame buffer
    // =============================================================
    //  Binary functions are used because frame buffer is not 
    //  guaranteed to be tryte-aligned.
    // =============================================================
    memset_BINARY(bootInfo->framebuffer->address, 0, bootInfo->framebuffer->size);

    print(&r, "Kernel initialized.\n");

    while(true);
}