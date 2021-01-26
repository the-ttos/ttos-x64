#ifndef EFIMEMORY_H
#define EFIMEMORY_H
#include "efimemory.h"
#endif

#ifndef FONTS_H
#define FONTS_H
#include "fonts.h"
#endif

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "framebuffer.h"
#endif

typedef struct {
	FRAMEBUFFER *framebuffer;
	PSF1_FONT *font;
	EFI_MEMORY_DESCRIPTOR *map;
	uint64_t mapSize;
	uint64_t mapDescriptorSize;
} BOOT_INFO;

extern BOOT_INFO *bootInfo;