#ifndef KERNEL_H
#define KERNEL_H
#include "kernel.h"
#endif

/*

    @@@@@@@  @@@@@@@   @@@   @@@@@@   @@@@@@@@   @@@@@@@  @@@@@@@  
    @@@@@@@  @@@@@@@@  @@@  @@@@@@@   @@@@@@@@  @@@@@@@@  @@@@@@@  
      @@!    @@!  @@@  @@!  !@@       @@!       !@@         @@!    
      !@!    !@!  @!@  !@!  !@!       !@!       !@!         !@!    
      @!!    @!@!!@!   !!@  !!@@!!    @!!!:!    !@!         @!!    
      !!!    !!@!@!    !!!   !!@!!!   !!!!!:    !!!         !!!    
      !!:    !!: :!!   !!:       !:!  !!:       :!!         !!:    
      :!:    :!:  !:!  :!:      !:!   :!:       :!:         :!:    
       ::    ::   :::   ::  :::: ::    :: ::::   ::: :::     ::    
       :      :   : :  :    :: : :    : :: ::    :: :: :     :     

*/

extern void _start(BOOT_INFO *bootInfo){
    // Renderer
    renderer = (RENDERER) {
        .target = bootInfo->framebuffer,
        .cursor = (POINT){0, 0},
        .font = bootInfo->font,
        .foreground = 0xffffffff,
        .background = 0x00000000
    };

    // Start kernel
    KERNEL_INFO kernelInfo = start_kernel(bootInfo);

    print("Kernel initialized.");

    int *test = (int*)0x80000000000;
    *test = 2;

    while(true);
}