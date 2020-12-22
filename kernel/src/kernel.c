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
    //renderer r = { f, { f->width / 2 - 8 * 49 / 2, f->height / 2 - 16 * 15 / 2 }, font, 0x2ecc71ff };
    for(unsigned i = 0; i < f->width * 4; i++)
        for(unsigned j = 0; j < f->height; j++)
            *(unsigned*)(i + (j * f->pixelsPerScanline * 4) + f->address) = 0x00000000;
    // print(&r, "\r\n                  ,----,                        \r\n                ,/   .`|  ,----..               \r\n    ___       ,`   .'  : /   /   \\   .--.--.    \r\n  ,--.'|_   ;    ;     //   .     : /  /    '.  \r\n  |  | :,'.'___,/    ,'.   /   ;.  \\  :  /`. /  \r\n  :  : ' :|    :     |.   ;   /  ` ;  |  |--`   \r\n.;__,'  / ;    |.';  ;;   |  ; \\ ; |  :  ;_     \r\n|  |   |  `----'  |  ||   :  | ; | '\\  \\    `.  \r\n:__,'| :      '   :  ;.   |  ' ' ' : `----.   \\ \r\n  '  : |__    |   |  ''   ;  \\; /  | __ \\  \\  | \r\n  |  | '.'|   '   :  | \\   \\  ',  / /  /`--'  / \r\n  ;  :    ;   ;   |.'   ;   :    / '--'.     /  \r\n  |  ,   /    '---'      \\   \\ .'    `--'---'   \r\n   ---`-'                 `---`                 \r\n                                                \r\n                                                \r\n");
    
    __new_tryte(foo) = 262142;

    print(&r, to_string(foo));
    print(&r, "\r\n");

}