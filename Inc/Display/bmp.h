// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.

#ifndef _BMP_H_
#define _BMP_H_

#include <stdint.h>
#include <cstring>

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
 #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
 #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

typedef struct {
	uint8_t  *buf;
	int16_t   width, height;
} Bitmap;

void drawBitmap(uint8_t *cbuf, int16_t x, int16_t y, const Bitmap *bmp) {
	if(cbuf && bmp) {
		uint8_t *buf = (uint8_t *)pgm_read_pointer(&bmp->buf);
		int16_t width = *(const int16_t *)&bmp->width;
		int16_t height = *(const int16_t *)&bmp->height;

		for (int i = 0; i < height; ++i) {
			uint8_t *ptr = &cbuf[(x / 8) + (y + i) * ((WIDTH + 7) / 8)];
			buf += i * width / 8;
        	memcpy(&ptr, &buf, sizeof(width / 8));
		}
    }
}

#endif // _BMP_H_
