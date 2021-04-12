#ifndef KERNEL_H
#define KERNEL_H
#include "kernel.h"
#endif

/*

        888            d8b                            888    
        888            Y8P                            888    
        888                                           888    
        888888 888d888 888 .d8888b   .d88b.   .d8888b 888888 
        888    888P"   888 88K      d8P  Y8b d88P"    888    
        888    888     888 "Y8888b. 88888888 888      888    
        Y88b.  888     888      X88 Y8b.     Y88b.    Y88b.  
         "Y888 888     888  88888P'  "Y8888   "Y8888P  "Y888 
                                                      
*/  

extern void _start(BOOT_INFO *bootInfo){
    // Start kernel
    KERNEL_INFO kernelInfo = start_kernel(bootInfo);
    
    // Renderer
    renderer = (RENDERER) {
        .target = bootInfo->framebuffer,
        .cursor = ANCHOR,
        .font = bootInfo->font,
        .color = 0xfffbc531
    };

    print(&renderer, "Kernel initialized.\n");

    int *test = (int*)0x8000000000000;
    *test = 2;

    while(true);
}