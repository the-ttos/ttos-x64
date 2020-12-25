typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char size;
} PSF1_HEADER;

typedef struct {
	PSF1_HEADER *header;
	void *glyphs;
} PSF1_FONT;