// PSF1 header structure
typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char size;
} PSF1_HEADER;

// PSF1 font structure
typedef struct {
	PSF1_HEADER *header;
	void *glyphs;
} PSF1_FONT;