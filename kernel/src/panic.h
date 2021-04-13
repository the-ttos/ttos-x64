#ifndef RENDERER_H
#define RENDERER_H
#include "renderer.h"
#endif

void panic(char *message) {
    clear(0xA3CB38);
    renderer.cursor = ANCHOR;
    renderer.foreground = 0x000000;
    printk(" KERNEL PANIC \n", message, 0, 0);
}