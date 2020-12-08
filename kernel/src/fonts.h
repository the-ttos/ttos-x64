typedef struct {
	unsigned char magic[2];
	unsigned char mode;
	unsigned char size;
} psf1Header;

typedef struct {
	psf1Header *header;
	void *glyphs;
} psf1Font;