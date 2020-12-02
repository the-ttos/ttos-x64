typedef unsigned long long size_t;

typedef struct {
	void *address;
	size_t size;
	unsigned width;
	unsigned height;
	unsigned pixelsPerScanline;
} frameBuffer;

typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char size;
} psf1Header;

typedef struct {
	psf1Header *header;
	void *glyphs;
} psf1Font;

void put_char(frameBuffer *f, psf1Font *font, unsigned color, char chr, unsigned xOff, unsigned yOff) {
    unsigned *pixelPtr = (unsigned*)f->address;
    char *fontPtr = font->glyphs + (chr * font->header->size);
    for(unsigned long y = yOff; y < yOff + 16; y++) {
        for(unsigned long x = xOff; x < xOff + 8; x++)
            if(*fontPtr & (0b10000000 >> (x - xOff)))
                *(unsigned*)(pixelPtr + x + (y * f->pixelsPerScanline)) = color;
        fontPtr++;
    }
}

void _start(frameBuffer *f, psf1Font *font){
    put_char(f, font, 0xffffffff, '☺', 10, 10);
    return;
}