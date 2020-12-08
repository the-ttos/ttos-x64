#include <stddef.h>

typedef struct {
	void *address;
	size_t size;
	unsigned width;
	unsigned height;
	unsigned pixelsPerScanline;
} frameBuffer;