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

typedef struct {
    unsigned x;
    unsigned y;
} point;

void put_char(frameBuffer *f, psf1Font *font, unsigned color, char chr, unsigned long xOff, unsigned long yOff) {
    unsigned *pixelPtr = (unsigned*)f->address;
    char *fontPtr = font->glyphs + (chr * font->header->size);
    for(unsigned long y = yOff; y < yOff + 16; y++) {
        for(unsigned long x = xOff; x < xOff + 8; x++)
            if(*fontPtr & (0b10000000 >> (x - xOff)))
                *(unsigned*)(pixelPtr + x + (y * f->pixelsPerScanline)) = color;
        fontPtr++;
    }
}

point cursor;
void print(frameBuffer *f, psf1Font *font, unsigned color, const char *str) {
    unsigned x = cursor.x;
    while(*str) {
        switch(*str) {
            case '\n':
                cursor.y += 16;
                goto next;
            case '\r':
                cursor.x = x;
                goto next;
        }
        put_char(f, font, color, *str, cursor.x, cursor.y);
        cursor.x += 8;
        if(cursor.x + 8 > f->width) {
            cursor.x = x;
            cursor.y += 16;
        }
        next:
        str++;
    }
}

void _start(frameBuffer *f, psf1Font *font){
    cursor = (point){f->width / 2 - 8 * 40, f->height / 2 - 16 * 5};
    for(unsigned i = 0; i < f->width * 4; i++)
        for(unsigned j = 0; j < f->height; j++)
            *(unsigned*)(i + (j * f->pixelsPerScanline * 4) + f->address) = 0x00000000;
    print(f, font, 0xf1c40f, "\r\n________________/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\_______/\\\\\\\\\\__________/\\\\\\\\\\\\\\\\\\\\\\___        \r\n _______________\\///////\\\\\\/////______/\\\\\\///\\\\\\______/\\\\\\/////////\\\\\\_       \r\n  _____/\\\\\\____________\\/\\\\\\_________/\\\\\\/__\\///\\\\\\___\\//\\\\\\______\\///__      \r\n   __/\\\\\\\\\\\\\\\\\\\\\\_______\\/\\\\\\________/\\\\\\______\\//\\\\\\___\\////\\\\\\_________     \r\n    _\\////\\\\\\////________\\/\\\\\\_______\\/\\\\\\_______\\/\\\\\\______\\////\\\\\\______    \r\n     ____\\/\\\\\\____________\\/\\\\\\_______\\//\\\\\\______/\\\\\\__________\\////\\\\\\___   \r\n      ____\\/\\\\\\_/\\\\________\\/\\\\\\________\\///\\\\\\__/\\\\\\_____/\\\\\\______\\//\\\\\\__  \r\n       ____\\//\\\\\\\\\\_________\\/\\\\\\__________\\///\\\\\\\\\\/_____\\///\\\\\\\\\\\\\\\\\\\\\\/___ \r\n        _____\\/////__________\\///_____________\\/////_________\\///////////_____\r\n");
    while(1);
}