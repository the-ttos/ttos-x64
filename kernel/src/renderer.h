#ifndef MATH_H
#define MATH_H
#include "math.h"
#endif

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "framebuffer.h"
#endif

#ifndef FONTS_H
#define FONTS_H
#include "fonts.h"
#endif

typedef struct {
    frameBuffer *target;
    point cursor;
    psf1Font *font;
    unsigned color;
} renderer;

void put_char(renderer *r, char chr, unsigned long xOff, unsigned long yOff) {
    unsigned *pixelPtr = (unsigned*)r->target->address;
    char *fontPtr = r->font->glyphs + (chr * r->font->header->size);
    for(unsigned long y = yOff; y < yOff + 16; y++) {
        for(unsigned long x = xOff; x < xOff + 8; x++)
            if(*fontPtr & (0b10000000 >> (x - xOff)))
                *(unsigned*)(pixelPtr + x + (y * r->target->pixelsPerScanline)) = r->color;
        fontPtr++;
    }
}

void print(renderer *r, const char *str) {
    unsigned x = r->cursor.x;
    while(*str) {
        switch(*str) {
            case '\n':
                r->cursor.y += 16;
                goto next;
            case '\r':
                r->cursor.x = x;
                goto next;
        }
        put_char(r, *str, r->cursor.x, r->cursor.y);
        r->cursor.x += 8;
        if(r->cursor.x + 8 > r->target->width) {
            r->cursor.x = x;
            r->cursor.y += 16;
        }
        next:
        str++;
    }
}

// Test
void print_grad(renderer *r, const char *str) {
    unsigned x = r->cursor.x;
    while(*str) {
        switch(*str) {
            case '\n':
                r->cursor.y += 16;
                goto next;
            case '\r':
                r->cursor.x = x;
                goto next;
        }
        if(*str != ' ') r->color++;
        put_char(r, *str, r->cursor.x, r->cursor.y);
        r->cursor.x += 8;
        if(r->cursor.x + 8 > r->target->width) {
            r->cursor.x = x;
            r->cursor.y += 16;
        }
        next:
        str++;
    }
}