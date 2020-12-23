#ifndef RENDERER_H
#define RENDERER_H
#include "renderer.h"
#endif

#ifndef CSTR_H
#define CSTR_H
#include "cstr.h"
#endif

#ifndef FONTS_H
#define FONTS_H
#include "fonts.h"
#endif

#ifndef TRYTE_H
#define TRYTE_H
#include "tryte.h"
#endif

void _start(frameBuffer *f, psf1Font *font){
    renderer r = {f, ANCHOR, font, 0x2ecc71ff};
    /*
    for(unsigned i = 0; i < f->width * 4; i++)
        for(unsigned j = 0; j < f->height; j++)
            *(unsigned*)(i + (j * f->pixelsPerScanline * 4) + f->address) = 0x00000000;
    */
   
    __tryte(t) = {0b10001001, 0b10010100, 0b10000000};
    print(&r, "ID  t (27): ");
    print(&r, tryte_to_hstring(t));
    print(&r, "\n");
    print(&r, "ID  t (10): ");
    print(&r, tryte_to_string(t));
    print(&r, "\n");
    print(&r, "ID  t  (3): ");
    print(&r, tryte_to_tstring(t));
    print(&r, "\n");
    print(&r, "NOT t (27): ");
    print(&r, tryte_to_hstring(__not(t)));
    print(&r, "\n");
    print(&r, "NOT t (10): ");
    print(&r, tryte_to_string(__not(t)));
    print(&r, "\n");
    print(&r, "NOT t  (3): ");
    print(&r, tryte_to_tstring(__not(t)));
    print(&r, "\n");
    print(&r, "INC t (27): ");
    print(&r, tryte_to_hstring(__inc(t)));
    print(&r, "\n");
    print(&r, "INC t (10): ");
    print(&r, tryte_to_string(__inc(t)));
    print(&r, "\n");
    print(&r, "INC t  (3): ");
    print(&r, tryte_to_tstring(__inc(t)));
    print(&r, "\n");
    print(&r, "DEC t (27): ");
    print(&r, tryte_to_hstring(__dec(t)));
    print(&r, "\n");
    print(&r, "DEC t (10): ");
    print(&r, tryte_to_string(__dec(t)));
    print(&r, "\n");
    print(&r, "DEC t  (3): ");
    print(&r, tryte_to_tstring(__dec(t)));
    print(&r, "\n");
}