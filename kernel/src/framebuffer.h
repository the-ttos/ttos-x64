#ifndef STDDEF_H
#define STDDEF_H
#include <stddef.h>
#endif

// Frame buffer structure
typedef struct {
	void *address;
	size_t size;
	unsigned width;
	unsigned height;
	unsigned pixelsPerScanline;
} FRAMEBUFFER;