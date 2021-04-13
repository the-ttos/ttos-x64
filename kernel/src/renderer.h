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

#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif


#define ANCHOR (POINT){16, 16}

// Renderer structure
typedef struct {
    FRAMEBUFFER *target;
    POINT cursor;
    PSF1_FONT *font;
    uint32_t foreground;
    uint32_t background;
} RENDERER;

// Global renderer
RENDERER renderer;

typedef enum {
    PIVOT_LEFT,
    PIVOT_CENTER,
    PIVOT_RIGHT
} PIVOT;

// Put a char on the screen
void put_char(char chr, uint64_t xOff, uint64_t yOff, bool invert) {
    uint32_t *pixelPtr = (uint32_t*)renderer.target->address;
    char *fontPtr = renderer.font->glyphs + (chr * renderer.font->header->size);
    for(uint64_t y = yOff; y < yOff + 16; y++) {
        for(uint64_t x = xOff; x < xOff + 8; x++)
            if(*fontPtr & (0b10000000 >> (x - xOff)))
                *(uint32_t*)(pixelPtr + x + (y * renderer.target->pixelsPerScanline)) = invert ? renderer.background : renderer.foreground;
            else if(invert) *(uint32_t*)(pixelPtr + x + (y * renderer.target->pixelsPerScanline)) = renderer.foreground;
        fontPtr++;
    }
}

// Get the length and height of text
static POINT get_text_size(const char *str) {
    POINT p = {0, 0};
    uint32_t x = 0;
    if(str) p.y++;
    while(*str) {
        if(*str == '\n') {
            if(p.x < x) p.x = x;
            if(str[1]) p.y++;
        } else p.x++;
        str++;
    }
    return (POINT){ p.x * 8, p.y * 16 };
}

static void reset_cursor(bool nextLine, const char *str, PIVOT pivot, POINT margin) {
    renderer.cursor.y = (renderer.cursor.y < margin.y ? margin.y : renderer.cursor.y);
    if(nextLine) renderer.cursor.y += 16;

    uint32_t c = 0;
    switch(pivot) {
        case PIVOT_LEFT:
            renderer.cursor.x = margin.x;
            break;
        case PIVOT_CENTER:
            while(str[c] && str[c] != '\n') c++;
            renderer.cursor.x = (renderer.target->width - c * 8) / 2;
            break;
    }
}

// Print a string to the screen using pivot
void printc(const char *str, PIVOT pivot, POINT offset, bool invert) {
    reset_cursor(false, str, pivot, ANCHOR);
    POINT anchor = (POINT){
        .x = renderer.cursor.x + offset.x,
        .y = renderer.cursor.y + offset.y
    };
    renderer.cursor = anchor;

    while(*str) {
        switch(*str) {
            case '\n':
                reset_cursor(true, str+1, pivot, anchor);
                break;
            default:
                put_char(*str, renderer.cursor.x, renderer.cursor.y, invert);
                renderer.cursor.x += 8;
                if(renderer.cursor.x + 8 > renderer.target->width)
                    reset_cursor(true, str, pivot, anchor);
                break;
        }
        str++;
    }
}

// Print a string to the screen
void print(const char *str) {
    printc(
        str, PIVOT_LEFT,
        (POINT){0, 0},
        false
    );
}

// Print a kernel message
void printk(const char *title, const char *str, uint32_t foreground, uint32_t background) {
    POINT titleSize = get_text_size(title);
    POINT strSize = get_text_size(str);

    printc(title,
        PIVOT_CENTER,
        (POINT){
            .x = 0, 
            .y = (renderer.target->height - titleSize.y - strSize.y - 10) / 2 - titleSize.y
        },
        true
    );
    printc(str,
        PIVOT_CENTER,
        (POINT){0, 10 / 2},
        false
    );
}

// Set the whole screen to specified color
void clear(uint32_t color) {
    uint64_t fbBase = (uint64_t)renderer.target->address;
    uint64_t bytesPerScanLine = renderer.target->pixelsPerScanline * 4;
    uint64_t fbHeight = renderer.target->height;
    uint64_t fbSize = renderer.target->size;

    for(uint32_t i = 0; i < fbHeight; i++) {
        uint64_t pixelPointerBase = fbBase + (bytesPerScanLine * i);
        for(uint32_t *pixelPointer = (uint32_t*)pixelPointerBase;
            pixelPointer < (uint32_t*)(pixelPointerBase + bytesPerScanLine);
            pixelPointer++) {
            *pixelPointer = color;
        }

    }
    renderer.background = color;
}
