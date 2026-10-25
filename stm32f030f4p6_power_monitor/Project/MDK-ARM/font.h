#ifndef __MYFONT_H__
#define __MYFONT_H__

#include <stdint.h>

/* Extracted from LVGL font.c (STZHONGS 16px bpp=1).
 * bit-packed glyphs, independent of LVGL. */
typedef struct {
    uint32_t unicode;   /* character code */
    uint16_t bm;        /* bit offset into font_bits */
    uint8_t  w;         /* glyph pixel width */
    uint8_t  h;         /* glyph pixel height */
    int8_t   ox;        /* x offset */
    int8_t   oy;        /* y offset (negative = above baseline) */
} fglyph_t;

/* find glyph by unicode; returns NULL if not present */
const fglyph_t *myfont_find(uint32_t unicode);

/* pixel value (1/0) for glyph at local (gx, gy) */
int myfont_pixel(const fglyph_t *g, int gx, int gy);

/* cell height used by renderer */
#define MYFONT_CELL 16

#endif