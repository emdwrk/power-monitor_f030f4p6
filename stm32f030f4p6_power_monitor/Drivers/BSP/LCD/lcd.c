#include "./BSP/LCD/lcd.h"

static uint16_t ui_line_buf[LCD_W];

/* ================= Unicode font (16px bpp=1) ================= */

#define FONT_N 62

static const uint8_t font_bits[] = {
    0x38, 0x20, 0xD8, 0xC1, 0x11, 0x02, 0x24, 0x04, 0x48, 0x08, 0xA7, 0x1A, 0x5B, 0x1D, 0x22, 0x02,
    0x44, 0x08, 0x88, 0x31, 0x90, 0x41, 0xC0, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x03, 0x08, 0x23, 0x00,
    0x00, 0x00, 0x0C, 0x20, 0x1C, 0x1F, 0x18, 0xCC, 0x66, 0x33, 0x09, 0x84, 0xC6, 0x63, 0x31, 0x8F,
    0x83, 0x80, 0x1C, 0x1E, 0x03, 0x01, 0x80, 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x0F, 0xCF, 0xE0,
    0x3C, 0x3F, 0x10, 0xC0, 0x60, 0x30, 0x30, 0x18, 0x18, 0x18, 0x18, 0x1F, 0xCF, 0xE0, 0x3C, 0x3F,
    0x00, 0xC0, 0x60, 0x60, 0xE0, 0x38, 0x06, 0x03, 0x21, 0x9F, 0xC7, 0x80, 0x06, 0x07, 0x07, 0x83,
    0xC3, 0x61, 0x31, 0x99, 0xCE, 0xFF, 0x83, 0x01, 0x80, 0xC0, 0x3F, 0x1F, 0x08, 0x0C, 0x07, 0xE1,
    0x38, 0x0C, 0x06, 0x03, 0x21, 0x9F, 0x87, 0x80, 0x1E, 0x1F, 0x8C, 0x0C, 0x06, 0x03, 0xF1, 0xCC,
    0xC2, 0x61, 0x31, 0x8D, 0xC3, 0xC0, 0x7F, 0x3F, 0x80, 0x80, 0xC0, 0x40, 0x60, 0x30, 0x10, 0x18,
    0x0C, 0x06, 0x03, 0x00, 0x1C, 0x1B, 0x18, 0xCC, 0x63, 0x30, 0xF0, 0xF8, 0xC6, 0x41, 0x31, 0x9D,
    0xC3, 0xC0, 0x1C, 0x3F, 0x18, 0xC8, 0x64, 0x33, 0x18, 0xFC, 0x06, 0x03, 0x01, 0x1F, 0x87, 0x80,
    0x0C, 0x07, 0x81, 0xE0, 0x48, 0x33, 0x0C, 0xC2, 0x31, 0xFC, 0x61, 0x98, 0x6C, 0x0B, 0x03, 0x3F,
    0x06, 0xF0, 0x86, 0x10, 0xC2, 0x18, 0x7E, 0x0C, 0xE1, 0x06, 0x20, 0xC4, 0x18, 0xCE, 0x1F, 0x80,
    0x0F, 0x07, 0xE3, 0x01, 0x80, 0x60, 0x18, 0x06, 0x01, 0x80, 0x60, 0x0C, 0x21, 0xF8, 0x3C, 0x3E,
    0x07, 0xF0, 0x86, 0x10, 0x62, 0x0C, 0x41, 0x88, 0x31, 0x06, 0x20, 0xC4, 0x30, 0xFE, 0x1F, 0x00,
    0x3F, 0x0F, 0xC2, 0x00, 0x80, 0x20, 0x0F, 0xC3, 0xE0, 0x80, 0x20, 0x08, 0x03, 0xF0, 0xFE, 0x3F,
    0x9F, 0x88, 0x04, 0x02, 0x01, 0xF0, 0xFC, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, 0x0F, 0x03, 0xF8,
    0xC0, 0x30, 0x06, 0x00, 0xC3, 0x98, 0xF3, 0x06, 0x60, 0xC6, 0x18, 0x7F, 0x07, 0xC0, 0x20, 0xC2,
    0x0C, 0x20, 0xC2, 0x0C, 0x20, 0xC3, 0xFC, 0x3F, 0xC2, 0x0C, 0x20, 0xC2, 0x0C, 0x20, 0xC2, 0x0C,
    0x21, 0x08, 0x42, 0x10, 0x84, 0x21, 0x08, 0x40, 0x06, 0x03, 0x01, 0x80, 0xC0, 0x60, 0x30, 0x18,
    0x0C, 0x06, 0x23, 0x1F, 0x87, 0x80, 0x20, 0xC4, 0x30, 0x8C, 0x13, 0x02, 0xE0, 0x7C, 0x0F, 0x81,
    0x98, 0x21, 0x84, 0x30, 0x83, 0x10, 0x60, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x80, 0x40,
    0x20, 0x10, 0x0F, 0xC7, 0xE0, 0x30, 0x71, 0x83, 0x8E, 0x1C, 0x71, 0xE2, 0x8B, 0x16, 0x58, 0xB6,
    0xC4, 0xA6, 0x27, 0x31, 0x19, 0x88, 0x8C, 0x40, 0x60, 0x30, 0xC3, 0x0C, 0x38, 0xC2, 0x8C, 0x2C,
    0xC2, 0x4C, 0x26, 0xC2, 0x34, 0x23, 0x42, 0x1C, 0x21, 0xC2, 0x0C, 0x0F, 0x01, 0xFC, 0x30, 0xC6,
    0x06, 0x60, 0x66, 0x06, 0x60, 0x66, 0x06, 0x60, 0x63, 0x0C, 0x1F, 0xC0, 0xF0, 0x3F, 0x0F, 0xE2,
    0x18, 0x83, 0x20, 0x88, 0x63, 0xF0, 0xC0, 0x20, 0x08, 0x02, 0x00, 0x80, 0x0F, 0x01, 0xFC, 0x30,
    0xC6, 0x06, 0x60, 0x66, 0x06, 0x60, 0x66, 0x06, 0x60, 0x63, 0x0C, 0x3F, 0xC0, 0xF0, 0x07, 0x00,
    0x3E, 0x00, 0xE0, 0x3F, 0x07, 0xF0, 0x82, 0x10, 0x62, 0x18, 0x6F, 0x0F, 0xC1, 0x18, 0x23, 0x04,
    0x30, 0x86, 0x10, 0x60, 0x1E, 0x0F, 0xC6, 0x01, 0x80, 0x30, 0x07, 0x80, 0xF0, 0x0E, 0x01, 0x90,
    0x67, 0xF0, 0x78, 0xFF, 0x9F, 0xE0, 0xC0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x0C, 0x03, 0x00,
    0xC0, 0x30, 0x60, 0xC6, 0x0C, 0x60, 0xC6, 0x0C, 0x60, 0xC6, 0x0C, 0x60, 0xC2, 0x0C, 0x30, 0xC3,
    0x0C, 0x1F, 0x80, 0xF0, 0x60, 0xCC, 0x18, 0xC3, 0x18, 0x43, 0x18, 0x23, 0x06, 0x60, 0xC8, 0x0B,
    0x01, 0xE0, 0x38, 0x03, 0x00, 0xC3, 0x0D, 0x8C, 0x36, 0x38, 0x98, 0xE2, 0x66, 0x99, 0x9A, 0x62,
    0x4D, 0x8D, 0x34, 0x3C, 0xD0, 0xF1, 0xC3, 0x87, 0x06, 0x1C, 0x61, 0x98, 0xC3, 0x30, 0xD8, 0x1E,
    0x07, 0x01, 0xC0, 0x78, 0x36, 0x08, 0xC6, 0x31, 0x06, 0x60, 0xCC, 0x63, 0x18, 0x4C, 0x1B, 0x03,
    0x80, 0xE0, 0x30, 0x0C, 0x03, 0x00, 0xC0, 0x30, 0x7F, 0x8F, 0xE0, 0x30, 0x18, 0x06, 0x03, 0x00,
    0xC0, 0x60, 0x30, 0x0C, 0x07, 0xF9, 0xFE, 0x08, 0x20, 0x08, 0x60, 0x1F, 0xFE, 0x10, 0x60, 0x30,
    0x40, 0x33, 0xFC, 0x72, 0x04, 0x73, 0xFC, 0x32, 0x04, 0x33, 0xFC, 0x32, 0x04, 0x32, 0x04, 0x33,
    0xFC, 0x32, 0x04, 0x37, 0xFE, 0x10, 0x00, 0x00, 0x00, 0x00, 0x60, 0x7E, 0x60, 0x18, 0x60, 0x11,
    0xFE, 0x10, 0x66, 0x10, 0x66, 0x10, 0x66, 0x10, 0x46, 0x1A, 0x46, 0x1E, 0xC6, 0x78, 0xC6, 0x41,
    0x86, 0x03, 0x04, 0x06, 0x3C, 0x00, 0x00, 0x3F, 0xFE, 0x30, 0x00, 0x20, 0xC0, 0x20, 0xC0, 0x20,
    0xC0, 0x20, 0xC0, 0x2F, 0xFC, 0x20, 0xC0, 0x20, 0xD0, 0x20, 0xC8, 0x60, 0xCC, 0x60, 0xC0, 0x60,
    0xC0, 0x5F, 0xFE, 0x00, 0x00, 0x10, 0x40, 0x10, 0xC0, 0x10, 0xC0, 0x11, 0xFE, 0x7D, 0x86, 0x33,
    0x06, 0x12, 0x86, 0x10, 0xC6, 0x10, 0x66, 0x10, 0x06, 0x1C, 0x3E, 0x38, 0xE6, 0x71, 0x84, 0x41,
    0x04, 0x00, 0x0C, 0x00, 0x38, 0x00, 0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
    0x80, 0x7F, 0xFE, 0x7F, 0xFE, 0x03, 0xC0, 0x03, 0xC0, 0x02, 0x40, 0x06, 0x60, 0x0C, 0x30, 0x0C,
    0x38, 0x38, 0x1C, 0x70, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFC, 0x01, 0x80, 0x11, 0x88, 0x19,
    0x98, 0x09, 0x90, 0x09, 0x90, 0x01, 0x80, 0x7F, 0xFE, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
    0x80, 0x01, 0x80, 0x01, 0x80, 0x1F, 0xF8, 0x10, 0x08, 0x1F, 0xF8, 0x10, 0x08, 0x1F, 0xF8, 0x00,
    0x00, 0x7F, 0xFE, 0x33, 0x00, 0x3F, 0xFE, 0x33, 0x44, 0x3F, 0x6C, 0x33, 0x38, 0x3F, 0x38, 0x7B,
    0xEE, 0x02, 0x02, 0x00, 0x40, 0x30, 0x60, 0x38, 0x60, 0x07, 0xFE, 0x00, 0x88, 0x41, 0x8C, 0x73,
    0xFC, 0x13, 0x86, 0x00, 0x48, 0x03, 0x48, 0x1B, 0x48, 0x13, 0x48, 0x32, 0x4A, 0x66, 0x4A, 0x64,
    0x4E, 0x00, 0x00, 0x01, 0x00, 0x01, 0x80, 0x7F, 0xFE, 0x01, 0x00, 0x32, 0x6C, 0x17, 0xD8, 0x01,
    0x80, 0x1B, 0x78, 0x77, 0xEC, 0x45, 0x24, 0x01, 0x80, 0x7F, 0xFE, 0x01, 0x80, 0x01, 0x80, 0x01,
    0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x3F, 0xFC, 0x31, 0x8C, 0x31, 0x8C, 0x3F,
    0xFC, 0x31, 0x8C, 0x31, 0x8C, 0x31, 0x8C, 0x3F, 0xFC, 0x31, 0x80, 0x01, 0x82, 0x01, 0x82, 0x01,
    0xFE, 0x00, 0x00, 0x00, 0x20, 0x30, 0x20, 0x18, 0x20, 0x08, 0x20, 0x00, 0x20, 0x03, 0xFE, 0x7B,
    0xFE, 0x18, 0x20, 0x18, 0x20, 0x18, 0x20, 0x18, 0x20, 0x1A, 0x20, 0x1E, 0x20, 0x1C, 0x20, 0x10,
    0x20, 0x00, 0x20, 0x1F, 0xF8, 0x10, 0x08, 0x1F, 0xF8, 0x10, 0x08, 0x1F, 0xF8, 0x00, 0x00, 0x7F,
    0xFE, 0x1F, 0xF8, 0x11, 0x8C, 0x1F, 0xFC, 0x11, 0x8C, 0x1F, 0xF8, 0x3F, 0xFC, 0x01, 0x80, 0x7F,
    0xFE, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFC, 0x31, 0x8C, 0x3F, 0xFC, 0x31, 0x8C, 0x31, 0x8C, 0x3F,
    0xFC, 0x0E, 0x20, 0x1F, 0xC0, 0x07, 0x18, 0x0F, 0xFC, 0x3F, 0x84, 0x09, 0xB0, 0x19, 0x98, 0x71,
    0x8C, 0x03, 0x00, 0x00, 0x00, 0x63, 0xFC, 0x30, 0x00, 0x10, 0x00, 0x00, 0x00, 0x07, 0xFE, 0x70,
    0xC0, 0x30, 0x90, 0x11, 0x98, 0x11, 0x0C, 0x13, 0xFC, 0x13, 0xFE, 0x10, 0x00, 0x7C, 0x00, 0x67,
    0xFE, 0x00, 0x00, 0x08, 0x00, 0x19, 0xFE, 0x30, 0x00, 0x60, 0x00, 0x4C, 0x00, 0x0C, 0x00, 0x1B,
    0xFE, 0x38, 0x18, 0x70, 0x18, 0x50, 0x18, 0x10, 0x18, 0x10, 0x18, 0x10, 0x18, 0x10, 0x18, 0x10,
    0x78, 0x10, 0x00, 0x00, 0x08, 0x00, 0x0C, 0x7C, 0x0C, 0x64, 0x0C, 0x65, 0xFE, 0x64, 0x0C, 0x64,
    0x0C, 0x7C, 0x8C, 0x64, 0xCC, 0x64, 0x6C, 0x64, 0x0C, 0x7C, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x00,
    0x78, 0x00, 0x00, 0x00, 0x00, 0x33, 0xFE, 0x18, 0x06, 0x00, 0x06, 0x60, 0x06, 0x67, 0xE6, 0x64,
    0x26, 0x64, 0x26, 0x67, 0xE6, 0x64, 0x26, 0x64, 0x26, 0x64, 0x26, 0x67, 0xE6, 0x60, 0x06, 0x60,
    0x06, 0x60, 0x0C, 0x00, 0x00,
    0x92, 0x7F, 0x24, 0x92, 0x49, 0x24, 0x92, 0x49, 0x24, 0x79, 0x0A, 0x17, 0xE8, 0x50, 0x9D, 0x00,
    0x82, 0x08, 0x20, 0xFA, 0x28, 0xA2, 0x8A, 0x28, 0xA2,
};

static const uint8_t f_w[FONT_N] = {
    0, 0, 15, 6, 5, 5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    10, 11, 10, 11, 10, 9, 11, 12, 5, 9, 11, 9, 13, 12, 12, 10,
    12, 11, 10, 10, 12, 11, 14, 10, 10, 10, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 9, 7, 6,
};
static const uint8_t f_h[FONT_N] = {
    0, 0, 12, 6, 3, 9, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    15, 12, 12, 12, 12, 12, 12, 12, 12, 12, 16, 16, 15, 17, 16, 15,
    15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 8, 8, 12,
};
static const int8_t f_ox[FONT_N] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const int8_t f_oy[FONT_N] = {
    0, 19, 7, 13, 16, 10, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 6, 5, 5, 6,
    6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 11, 11, 7,
};
static const uint16_t f_bm[FONT_N] = {
    0, 0, 0, 23, 28, 30, 36, 50, 64, 78, 92, 106, 120, 134, 148, 162,
    176, 191, 208, 223, 240, 255, 269, 286, 304, 312, 326, 343, 357, 377, 395, 413,
    428, 451, 468, 483, 498, 516, 533, 554, 569, 584, 599, 631, 663, 693, 727, 759,
    789, 819, 851, 883, 915, 947, 979, 1011, 1043, 1075, 1107, 1141, 1150, 1157,
};
static const uint32_t f_uni[FONT_N] = {
    0x0000, 0x0020, 0x0025, 0x002D, 0x002E, 0x003A, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039,
    0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050,
    0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x503C, 0x529F, 0x538B, 0x5747, 0x5927, 0x5E73,
    0x6700, 0x6D41, 0x7387, 0x7535, 0x8BA1, 0x91CF, 0x7D2F, 0x8FD0, 0x884C, 0x65F6, 0x95F4, 0x006D, 0x0061, 0x0068,
};

static uint16_t font_find(uint32_t u)
{
    uint8_t i;
    for (i = 0; i < FONT_N; i++)
        if (f_w[i] && f_uni[i] == u) return i;
    return 0;
}

static uint8_t font_px(uint16_t id, uint8_t gx, uint8_t gy)
{
    uint32_t bit = (uint32_t)f_bm[id] * 8 + (uint32_t)(gy * f_w[id] + gx);
    return (uint8_t)((font_bits[bit >> 3] >> (7 - (bit & 7))) & 1);
}

static uint32_t u8(const char **pp)
{
    const uint8_t *s = (const uint8_t *)*pp;
    uint32_t c = s[0];
    if (c < 0x80) { *pp += 1; return c; }
    if ((c & 0xE0) == 0xC0) { *pp += 2; return ((c & 0x1F) << 6) | (s[1] & 0x3F); }
    if ((c & 0xF0) == 0xE0) { *pp += 3; return ((c & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F); }
    if ((c & 0xF8) == 0xF0) { *pp += 4; return ((c & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F); }
    *pp += 1;
    return c;
}

static uint16_t gput(uint16_t x, uint16_t y, uint32_t c, uint8_t s, uint16_t color)
{
    uint16_t id;
    uint8_t w, h, gx, gy;
    int16_t ty;

    if (c == ' ') return (uint16_t)(s * 4);
    id = font_find(c);
    if (!id) return (uint16_t)(s * 4);
    w = f_w[id]; h = f_h[id];
    ty = (int16_t)y + f_oy[id];

    if (s == 1)
    {
        for (gy = 0; gy < h; gy++)
        {
            int16_t py = ty + gy;
            if (py < 0) continue;
            if (py > (int16_t)(LCD_H - 1)) break;
            for (gx = 0; gx < w; gx++)
            {
                if (font_px(id, gx, gy))
                    LCD_DrawPoint((uint16_t)(x + f_ox[id] + gx), (uint16_t)py, color);
            }
        }
    }
    else
    {
        int16_t oy = (int16_t)f_oy[id] * (int16_t)s;
        for (gy = 0; gy < h; gy++)
        {
            int16_t py = (int16_t)y + oy + (int16_t)gy * (int16_t)s;
            if (py < 0) continue;
            if (py > (int16_t)(LCD_H - s)) break;
            for (gx = 0; gx < w; gx++)
            {
                if (!font_px(id, gx, gy)) continue;
                LCD_Fill((uint16_t)(x + (f_ox[id] + (int16_t)gx) * s),
                         (uint16_t)py,
                         (uint16_t)(x + (f_ox[id] + (int16_t)gx + 1) * s - 1),
                         (uint16_t)(py + s - 1), color);
            }
        }
    }
    return (uint16_t)((f_w[id] + 1) * s);
}

static uint16_t gadvance(const char *str, uint8_t s)
{
    const char *p = str;
    uint16_t a = 0;
    while (*p)
    {
        uint32_t c = u8(&p);
        if (c == ' ') { a += (uint16_t)(s * 4); continue; }
        {
            uint16_t id = font_find(c);
            a += (uint16_t)((f_w[id] + 1) * s);
        }
    }
    return a;
}

static uint16_t gputs(uint16_t x, uint16_t y, const char *str, uint8_t s, uint16_t color)
{
    const char *p = str;
    uint16_t xx = x;
    while (*p)
        xx += gput(xx, y, u8(&p), s, color);
    return xx;
}

/* ================= display primitives ================= */

void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_Address_Set(x, y, x, y);
    lcd_w_half_word(color);
}





/* ---- gradient fill via line buffer ---- */
static void LCD_FillGradBuf(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                            uint16_t c0, uint16_t c1, uint8_t n)
{
    uint16_t h  = y1 - y0;
    uint16_t bw = (uint16_t)(h / n);
    uint16_t y  = y0;
    uint8_t  i;
    uint16_t w  = x1 - x0;

    for (i = 0; i < n; i++)
    {
        uint16_t ye = (i == n - 1) ? y1 : (y + bw);
        uint16_t t  = (uint16_t)(((uint32_t)i * 255) / (n - 1));
        int16_t r = (int16_t)((c0 >> 11) & 0x1F) + (int16_t)(((int)((c1 >> 11) & 0x1F) - (int)((c0 >> 11) & 0x1F)) * t >> 8);
        int16_t g = (int16_t)((c0 >> 5)  & 0x3F) + (int16_t)(((int)((c1 >> 5)  & 0x3F) - (int)((c0 >> 5)  & 0x3F)) * t >> 8);
        int16_t b = (int16_t)(c0         & 0x1F) + (int16_t)(((int)((c1         & 0x1F) - (int)((c0         & 0x1F))) * t >> 8));
        uint16_t col;
        uint16_t px;
        uint32_t rows;
        uint16_t row;

        if (r < 0) r = 0; if (r > 31) r = 31;
        if (g < 0) g = 0; if (g > 63) g = 63;
        if (b < 0) b = 0; if (b > 31) b = 31;
        col = (uint16_t)((r << 11) | (g << 5) | b);

        for (px = 0; px < w; px++) ui_line_buf[px] = col;
        rows = (uint16_t)(ye - y);
        LCD_Address_Set(x0, y, (uint16_t)(x0 + w - 1), (uint16_t)(ye - 1));
        for (row = 0; row < rows; row++)
        {
            for (px = 0; px < w; px++)
                lcd_w_half_word(ui_line_buf[px]);
        }
        y = ye;
    }
}

/* ================= UI ================= */

#define HDR_H   24
#define CARD_X  4
#define CARD_W  162
#define BAR_W   4

static char     g_txt[12][12];
static uint16_t g_lastw[12];

static uint16_t lerp565(uint16_t a, uint16_t b, uint8_t t)
{
    int r0 = (a >> 11) & 0x1F, g0 = (a >> 5) & 0x3F, b0 = a & 0x1F;
    int r1 = (b >> 11) & 0x1F, g1 = (b >> 5) & 0x3F, b1 = b & 0x1F;
    int r = r0 + (((r1 - r0) * (int)t) >> 8);
    int g = g0 + (((g1 - g0) * (int)t) >> 8);
    int bb = b0 + (((b1 - b0) * (int)t) >> 8);
    return (uint16_t)((r << 11) | (g << 5) | bb);
}

/* horizontal gradient fill (left->right), used for colour-rich bars */
static void LCD_FillGradH(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                          uint16_t c0, uint16_t c1)
{
    uint16_t w = x1 - x0;
    uint8_t  n = 8;
    uint16_t sw, x = x0;
    uint8_t  i;

    if (w == 0) return;
    sw = (w > n) ? (uint16_t)(w / n) : 1;
    for (i = 0; i < n; i++)
    {
        uint16_t xe = (i == n - 1) ? x1 : (uint16_t)(x + sw);
        uint16_t t  = (uint16_t)((i * 255) / (n - 1));
        uint16_t col;
        if (xe > x1) xe = x1;
        if (xe <= x) break;
        col = lerp565(c0, c1, (uint8_t)t);
        LCD_Fill(x, y0, (uint16_t)(xe - 1), y1, col);
        x = xe;
    }
}

static int same_str(const char *a, const char *b)
{
    while (*a && *b) { if (*a != *b) return 0; a++; b++; }
    return (*a == 0 && *b == 0);
}

static void copy_str(char *d, const char *s)
{
    uint8_t i = 0;
    while (s[i] && i < 11) { d[i] = s[i]; i++; }
    d[i] = 0;
}

static void sapp(char *b, char c)
{
    uint8_t i = 0;
    while (b[i]) i++;
    b[i] = c;
    b[i + 1] = 0;
}

static void fmt_num(char *b, int32_t v, uint8_t dec)
{
    uint32_t u, div = 1, ip, fp, scale = 1;
    uint8_t n = 0, d, shift;
    char t[12];
    uint8_t tn = 0;

    if (v < 0) { b[n++] = '-'; u = (uint32_t)(-v); }
    else       u = (uint32_t)v;

    shift = 4 - dec;
    while (shift--) scale *= 10;
    u = (u + scale / 2u) / scale;

    for (d = 0; d < dec; d++) div *= 10;
    ip = u / div;
    fp = u % div;

    if (ip == 0) t[tn++] = '0';
    else { while (ip) { t[tn++] = (char)('0' + ip % 10); ip /= 10; } }
    while (tn) b[n++] = t[--tn];

    if (dec)
    {
        b[n++] = '.';
        while (div > 1)
        {
            div /= 10;
            b[n++] = (char)('0' + (fp / div) % 10);
        }
    }
    b[n] = 0;
}

static void fmt_mah(char *b, int32_t v)
{
    if (v < 0) v = 0;
    if (v < 1000000)       fmt_num(b, v, 3);
    else if (v < 10000000) fmt_num(b, v, 2);
    else                   fmt_num(b, v, 1);
}

static void fmt_time(char *b, uint32_t s)
{
    uint32_t h = s / 3600;
    uint32_t m = (s / 60) % 60;
    uint32_t ss = s % 60;
    if (h > 99) h = 99;
    b[0] = (char)('0' + h / 10);
    b[1] = (char)('0' + h % 10);
    b[2] = ':';
    b[3] = (char)('0' + m / 10);
    b[4] = (char)('0' + m % 10);
    b[5] = ':';
    b[6] = (char)('0' + ss / 10);
    b[7] = (char)('0' + ss % 10);
    b[8] = 0;
}

static void ui_reset(void)
{
    uint8_t i;
    for (i = 0; i < 12; i++) { g_txt[i][0] = 0; g_lastw[i] = 0; }
}

static uint16_t ui_chw(uint8_t c, uint8_t sc)
{
    uint16_t id;
    if (c == ' ') return (uint16_t)(sc * 4);
    id = font_find(c);
    return (uint16_t)((f_w[id] + 1) * sc);
}

/*
 * Digit-level incremental refresh:
 *  - returns 0 without touching the screen when the string is unchanged;
 *  - redraws only the changed character cells when the length is stable,
 *    so constant digits never flicker;
 *  - full redraw only when the integer part grows/shrinks (length change).
 * Returns 1 when anything changed.
 */
static uint8_t ui_field(uint8_t id, uint16_t bx, uint16_t by, uint16_t bw,
                        uint8_t sc, uint8_t al, uint16_t fg, uint16_t bg, const char *s)
{
    uint16_t w, cw, cx, cy, ch, tx;
    uint16_t olen = 0, nlen = 0, i;

    if (same_str(g_txt[id], s)) return 0;

    while (g_txt[id][olen]) olen++;
    while (s[nlen]) nlen++;

    w = gadvance(s, sc);
    if (w > bw) w = bw;
    cw = (g_lastw[id] > w) ? g_lastw[id] : w;
    g_lastw[id] = w;

    if (sc == 2) { cy = (uint16_t)(by + 12); ch = 28; }
    else         { cy = (uint16_t)(by + 5);  ch = 16; }

    cx = bx;
    if (al == 1)      cx = (uint16_t)(bx + (bw - cw) / 2);
    else if (al == 2) cx = (uint16_t)(bx + bw - cw);

    tx = bx;
    if (al == 1)      tx = (uint16_t)(bx + (bw - w) / 2);
    else if (al == 2) tx = (uint16_t)(bx + bw - w);

    if (olen != nlen)
    {
        LCD_Fill(cx, cy, (uint16_t)(cx + cw + 2), (uint16_t)(cy + ch), bg);
        gputs(tx, by, s, sc, fg);
    }
    else
    {
        uint16_t px = tx;
        for (i = 0; i < nlen; i++)
        {
            uint8_t oc = (uint8_t)g_txt[id][i];
            uint8_t nc = (uint8_t)s[i];
            if (oc != nc)
            {
                uint16_t wo = ui_chw(oc, sc);
                uint16_t wn = ui_chw(nc, sc);
                uint16_t wm = (wo > wn) ? wo : wn;
                LCD_Fill((uint16_t)(px - 1), cy, (uint16_t)(px + wm + 1), (uint16_t)(cy + ch), bg);
                gput(px, by, nc, sc, fg);
            }
            px += ui_chw(nc, sc);
        }
    }
    copy_str(g_txt[id], s);
    return 1;
}

static void ui_cloud(uint16_t x, uint16_t y)
{
    LCD_Fill((uint16_t)(x + 2), (uint16_t)(y + 4), (uint16_t)(x + 24), (uint16_t)(y + 8), CLOUD);
    LCD_Fill((uint16_t)(x + 6), (uint16_t)(y + 1), (uint16_t)(x + 12), (uint16_t)(y + 5), CLOUD);
    LCD_Fill((uint16_t)(x + 14), (uint16_t)(y + 2), (uint16_t)(x + 20), (uint16_t)(y + 6), CLOUD);
    LCD_Fill((uint16_t)(x + 3), (uint16_t)(y + 8), (uint16_t)(x + 23), (uint16_t)(y + 9), CLOUD_SH);
}

static void ui_sun(uint16_t x, uint16_t y)
{
    LCD_Fill((uint16_t)(x - 4), y, (uint16_t)(x + 4), y, YELLOW);
    LCD_Fill(x, (uint16_t)(y - 4), x, (uint16_t)(y + 4), YELLOW);
    LCD_Fill((uint16_t)(x - 3), (uint16_t)(y - 3), (uint16_t)(x + 3), (uint16_t)(y + 3), YELLOW);
}

static void ui_battery(uint16_t x, uint16_t y, uint8_t loaded)
{
    LCD_Fill((uint16_t)(x + 1), (uint16_t)(y + 11), (uint16_t)(x + 25), (uint16_t)(y + 12), CARD_SHAD);
    LCD_Fill(x, y, (uint16_t)(x + 21), (uint16_t)(y + 10), 0x39A7);
    LCD_Fill((uint16_t)(x + 21), (uint16_t)(y + 3), (uint16_t)(x + 24), (uint16_t)(y + 7), 0x39A7);
    LCD_Fill((uint16_t)(x + 1), (uint16_t)(y + 1), (uint16_t)(x + 20), (uint16_t)(y + 9), loaded ? 0x0540 : 0x2104);
    LCD_Fill((uint16_t)(x + 2), (uint16_t)(y + 2), (uint16_t)(x + 6), (uint16_t)(y + 3), WHITE);
    LCD_Fill((uint16_t)(x + 1), (uint16_t)(y + 1), (uint16_t)(x + 1), (uint16_t)(y + 4), WHITE);
    if (loaded)
    {
        LCD_Fill((uint16_t)(x + 13), (uint16_t)(y + 1), (uint16_t)(x + 16), (uint16_t)(y + 4), YELLOW);
        LCD_Fill((uint16_t)(x + 9),  (uint16_t)(y + 4), (uint16_t)(x + 14), (uint16_t)(y + 6), YELLOW);
        LCD_Fill((uint16_t)(x + 6),  (uint16_t)(y + 6), (uint16_t)(x + 9),  (uint16_t)(y + 9), YELLOW);
    }
    else
    {
        uint8_t k;
        for (k = 0; k < 7; k++)
            LCD_Fill((uint16_t)(x + 5 + k), (uint16_t)(y + 7 - k), (uint16_t)(x + 5 + k), (uint16_t)(y + 7 - k), 0x8410);
    }
}


/* 实心山：apex(cx,apex_y) 到底边 base_y，右侧带阴影面 */
static void ui_mountain(uint16_t cx, uint16_t apex_y, uint16_t base_y,
                        uint16_t halfw, uint16_t col, uint16_t sh)
{
    uint16_t h = (uint16_t)(base_y - apex_y);
    uint16_t i;
    if (h == 0) return;
    for (i = 0; i <= h; i++)
    {
        uint16_t ww = (uint16_t)((uint32_t)halfw * i / h);
        uint16_t y  = (uint16_t)(apex_y + i);
        LCD_Fill((uint16_t)(cx - ww), y, (uint16_t)(cx + ww), y, col);
        if (sh != col)
            LCD_Fill((uint16_t)(cx + ww / 2), y, (uint16_t)(cx + ww), y, sh);
    }
}

/* 河流：y0->y1，宽度 w_top -> w_bot 渐变 */
static void ui_river(uint16_t cx, uint16_t y0, uint16_t y1,
                     uint16_t w_top, uint16_t w_bot, uint16_t col)
{
    uint16_t y;
    uint16_t span = (uint16_t)(y1 - y0);
    for (y = y0; y <= y1; y++)
    {
        uint16_t w = (span == 0) ? w_bot :
                     (uint16_t)(w_top + (uint32_t)(w_bot - w_top) * (y - y0) / span);
        LCD_Fill((uint16_t)(cx - w / 2), y, (uint16_t)(cx + w / 2), y, col);
    }
}

/* 河面波光：随 phase 流动的高亮短线 */
static void ui_sparkle(uint16_t cx, uint16_t y0, uint16_t y1,
                       uint16_t w, uint8_t phase, uint16_t col)
{
    uint16_t y;
    for (y = y0; y <= y1; y++)
    {
        if (((y + phase) & 3) == 0)
        {
            uint16_t ww = (uint16_t)(w / 3);
            uint16_t off = (uint16_t)((uint32_t)(y * 5 + phase * 7) % w);
            uint16_t sx = (uint16_t)(cx - w / 2 + off);
            uint16_t ex = (uint16_t)(sx + ww);
            if (ex > (uint16_t)(cx + w / 2)) ex = (uint16_t)(cx + w / 2);
            LCD_Fill(sx, y, ex, y, col);
        }
    }
}

static void ui_header_common(uint16_t c0, uint16_t c1, uint8_t dusk)
{
    const char *t = "\xE5\x8A\x9F\xE7\x8E\x87\xE8\xAE\xA1";
    uint16_t tw = gadvance(t, 1);
    LCD_FillGradBuf(0, 0, LCD_W, HDR_H, c0, c1, 4);
    if (dusk)
    {
        LCD_Fill(140, 4, 152, 12, 0xFD40);
        LCD_Fill(143, 2, 149, 14, 0xFD40);
    }
    else
    {
        ui_cloud(23, 2);
        ui_cloud(112, 2);
        ui_sun(146, 7);
    }
    gputs((uint16_t)((LCD_W - tw) / 2), 3, t, 1, WHITE);
    LCD_Fill(0, (uint16_t)(HDR_H - 1), LCD_W, HDR_H, 0x05BF);
}

static void ui_header(void) { ui_header_common(HEADER_BG, SKY_TOP, 0); }
static void ui_header_dusk(void) { ui_header_common(0x4013, 0xFD40, 1); }

static void ui_live(uint8_t on)
{
    LCD_Fill(156, 9, 162, 14, on ? GREEN : 0x18C6);
}

static void ui_dots(uint8_t page)
{
    uint8_t i;
    for (i = 0; i < 2; i++)
    {
        uint16_t x = (uint16_t)(5 + i * 10);
        LCD_Fill(x, 9, (uint16_t)(x + 5), 14, (i == page) ? ACCENT_W : 0x39A7);
    }
}

static void ui_card(uint16_t y, uint16_t h, uint16_t ac, uint16_t ac2,
                    const char *label, const char *unit)
{
    uint16_t uw;
    LCD_Fill(CARD_X, y, (uint16_t)(CARD_X + CARD_W), (uint16_t)(y + h), CARD_BG);
    LCD_FillGradBuf(CARD_X, y, (uint16_t)(CARD_X + BAR_W), (uint16_t)(y + h), ac, ac2, 4);
    LCD_Fill(CARD_X, y, (uint16_t)(CARD_X + CARD_W), (uint16_t)(y + 2), ac2);
    LCD_Fill((uint16_t)(CARD_X + CARD_W - 2), y, (uint16_t)(CARD_X + CARD_W), (uint16_t)(y + h), CARD_SHAD);
    LCD_Fill(CARD_X, (uint16_t)(y + h - 2), (uint16_t)(CARD_X + CARD_W), (uint16_t)(y + h), CARD_SHAD);
    gputs((uint16_t)(CARD_X + BAR_W + 6), (uint16_t)(y + 4), label, 1, TEXT_MID);
    if (unit)
    {
        uw = gadvance(unit, 1);
        gputs((uint16_t)(CARD_X + CARD_W - uw - 6), (uint16_t)(y + 4), unit, 1, ac2);
    }
}

static void ui_pbar(uint16_t x, uint16_t y, uint16_t w, uint16_t pct, uint16_t ac)
{
    uint16_t fw;
    if (pct > 100) pct = 100;
    fw = (uint16_t)((uint32_t)w * pct / 100);
    LCD_Fill(x, y, (uint16_t)(x + w - 1), (uint16_t)(y + 2), CARD_SHAD);
    if (fw > 0)
        LCD_FillGradH(x, y, (uint16_t)(x + fw), (uint16_t)(y + 3), ac, 0xFFFF);
}

static uint16_t heat_color(uint8_t t)
{
    if (t < 64)       return lerp565(BLUE,   CYAN,   (uint8_t)(t << 2));
    if (t < 128)      return lerp565(CYAN,   GREEN,  (uint8_t)((t - 64) << 2));
    if (t < 192)      return lerp565(GREEN,  YELLOW, (uint8_t)((t - 128) << 2));
    return lerp565(YELLOW, RED, (uint8_t)((t - 192) << 2));
}

static void ui_small(uint16_t x, uint16_t y, uint16_t ac, const char *label)
{
    LCD_Fill(x, y, (uint16_t)(x + 80), (uint16_t)(y + 60), CARD_BG);
    LCD_Fill(x, y, (uint16_t)(x + 80), (uint16_t)(y + 2), ac);
    LCD_Fill((uint16_t)(x + 78), y, (uint16_t)(x + 80), (uint16_t)(y + 60), CARD_SHAD);
    LCD_Fill(x, (uint16_t)(y + 58), (uint16_t)(x + 80), (uint16_t)(y + 60), CARD_SHAD);
    gputs((uint16_t)(x + 5), (uint16_t)(y + 5), label, 1, TEXT_MID);
}

static void ui_time_row(uint16_t y)
{
    LCD_Fill(CARD_X, y, (uint16_t)(CARD_X + CARD_W), (uint16_t)(y + 36), CARD_BG);
    LCD_Fill(CARD_X, y, (uint16_t)(CARD_X + CARD_W), (uint16_t)(y + 2), ACCENT_W2);
    LCD_Fill((uint16_t)(CARD_X + CARD_W - 2), y, (uint16_t)(CARD_X + CARD_W), (uint16_t)(y + 36), CARD_SHAD);
    LCD_Fill(CARD_X, (uint16_t)(y + 34), (uint16_t)(CARD_X + CARD_W), (uint16_t)(y + 36), CARD_SHAD);
    gputs(12, (uint16_t)(y + 9), "\xE8\xBF\x90\xE8\xA1\x8C\xE6\x97\xB6\xE9\x97\xB4", 1, TEXT_MID);
}

static void ui_footer(const char *s)
{
    uint16_t w = gadvance(s, 1);
    gputs((uint16_t)((LCD_W - w) / 2), 272, s, 1, TEXT_DIM);
}

static void ui_page1(void)
{
    LCD_FillGradBuf(0, 0, LCD_W, LCD_H, SKY_TOP, SKY_BOT, 8);
    ui_header();
    ui_dots(0);
    ui_card(24,  69, ACCENT_V,  ACCENT_V2, "\xE7\x94\xB5\xE5\x8E\x8B", "V");
    ui_card(94,  69, ACCENT_A,  ACCENT_A2, "\xE7\x94\xB5\xE6\xB5\x81", "A");
    ui_card(164, 69, ACCENT_W,  ACCENT_W2, "\xE5\x8A\x9F\xE7\x8E\x87", "W");
    ui_card(234, 60, ACCENT_W2, 0xFFE0,     "\xE7\x94\xB5\xE9\x87\x8F", "mAh");
}

static void ui_page2(void)
{
    LCD_FillGradBuf(0, 0, LCD_W, 150, 0x4013, 0xFD40, 8);
    LCD_FillGradBuf(0, 150, LCD_W, LCD_H, 0xFD40, 0x3206, 8);
    ui_header_dusk();
    ui_dots(1);
    ui_card(26, 68, ACCENT_W2, 0xFFE0, "\xE7\xB4\xAF\xE8\xAE\xA1\xE7\x94\xB5\xE9\x87\x8F", "mAh");
    ui_small(4,  98,  ACCENT_V, "\xE6\x9C\x80\xE5\xA4\xA7\xE7\x94\xB5\xE5\x8E\x8B");
    ui_small(86, 98,  ACCENT_A, "\xE6\x9C\x80\xE5\xA4\xA7\xE7\x94\xB5\xE6\xB5\x81");
    ui_small(4,  162, ACCENT_W, "\xE6\x9C\x80\xE5\xA4\xA7\xE5\x8A\x9F\xE7\x8E\x87");
    ui_small(86, 162, LIME,     "\xE5\xB9\xB3\xE5\x9D\x87\xE5\x8A\x9F\xE7\x8E\x87");
    ui_time_row(226);
    ui_footer("STATS");
}

static void ui_boot_scene(uint16_t sun_y, uint8_t spark, uint8_t titlet)
{
    const char *t = "\xE5\x8A\x9F\xE7\x8E\x87\xE8\xAE\xA1";
    uint16_t tw = gadvance(t, 2);

    LCD_FillGradBuf(0, 0, LCD_W, 215, SKY_DAWN, SUNRISE, 8);
    LCD_FillGradBuf(0, 215, LCD_W, LCD_H, SUNRISE, 0xFB20, 6);

    LCD_Fill(78, (uint16_t)(sun_y - 7), 92, (uint16_t)(sun_y + 7), 0xFF80);
    LCD_Fill(81, (uint16_t)(sun_y - 4), 89, (uint16_t)(sun_y + 4), YELLOW);

    ui_mountain(28, 134, 200, 40, MT_FAR, MT_FAR);
    ui_mountain(84, 124, 200, 50, MT_FAR, MT_FAR);
    ui_mountain(142, 138, 200, 44, MT_FAR, MT_FAR);

    ui_mountain(46, 178, 248, 58, MT_NEAR, MT_SHADE);
    ui_mountain(132, 186, 248, 62, MT_NEAR, MT_SHADE);

    ui_river(85, 248, 320, 8, 72, WATER);
    ui_sparkle(85, 252, 316, 68, spark, WATER_HI);

    if (titlet)
        gputs((uint16_t)((LCD_W - tw) / 2), 78, t, 2, lerp565(SUNRISE, WHITE, titlet));
}

void LCD_BootAnim(void)
{
    const char *sub = "POWER METER";
    const char *rd  = "READY";
    uint16_t sw = gadvance(sub, 1);
    uint16_t rw = gadvance(rd, 1);
    uint8_t i;

    for (i = 0; i < 7; i++)
    {
        ui_boot_scene((uint16_t)(112 - i * 11), 0, 0);
        Delay_ms(85);
    }

    for (i = 1; i <= 9; i++)
    {
        ui_boot_scene(46, i, (uint8_t)(i * 28));
        Delay_ms(60);
    }

    gputs((uint16_t)((LCD_W - sw) / 2), 150, sub, 1, WHITE);

    LCD_Fill(8, 168, 162, 176, CARD_BG);
    for (i = 1; i <= 13; i++)
    {
        uint16_t w = (uint16_t)(150 * i / 13);
        LCD_FillGradH(10, 170, (uint16_t)(10 + w), 175, ACCENT_W, ACCENT_W2);
        Delay_ms(45);
    }

    gputs((uint16_t)((LCD_W - rw) / 2), 184, rd, 1, WHITE);
    Delay_ms(150);
}

static void draw_graph(const int32_t *hist, uint8_t hist_len, int32_t avg_w)
{
    const uint16_t gy = 295;
    const uint16_t gh = 25;
    const uint16_t gw = (uint16_t)(LCD_W - 4);
    uint8_t i;
    int32_t max_w = 0;
    int32_t min_w = 0x7FFFFFFF;
    int32_t den;
    uint16_t bwpx;

    if (!hist || hist_len == 0) return;

    for (i = 0; i < hist_len; i++)
    {
        int32_t hw = hist[i];
        if (hw > max_w) max_w = hw;
        if (hw < min_w) min_w = hw;
    }
    if (max_w < 5000) max_w = 5000;
    if (max_w - min_w < 1000) max_w = min_w + 1000;
    den = max_w - min_w;

    LCD_FillGradH(2, (uint16_t)(gy - 2), (uint16_t)(2 + gw), (uint16_t)(gy), SKY_BOT, WATER_MID);
    LCD_FillGradBuf(2, gy, (uint16_t)(2 + gw), (uint16_t)(gy + gh), WATER_MID, WATER, 4);
    LCD_Fill(2, (uint16_t)(gy + gh - 1), (uint16_t)(2 + gw), (uint16_t)(gy + gh), MT_SHADE);

    bwpx = (uint16_t)(gw / hist_len);
    if (bwpx < 2) bwpx = 2;

    for (i = 0; i < hist_len; i++)
    {
        uint16_t bar_x, bar_h, bar_y, col;
        int32_t num = hist[i] - min_w;
        uint8_t norm8;

        bar_x = (uint16_t)(2 + gw * i / hist_len);
        bar_h = (uint16_t)((uint32_t)num * (gh - 3) / (uint32_t)den);
        bar_y = (uint16_t)(gy + gh - 3 - bar_h + 1);
        norm8 = (uint8_t)((uint32_t)num * 255u / (uint32_t)den);
        col = heat_color(norm8);

        if (bar_h > 0)
        {
            LCD_Fill(bar_x, bar_y, (uint16_t)(bar_x + bwpx - 1), (uint16_t)(bar_y + bar_h), col);
            LCD_Fill(bar_x, bar_y, (uint16_t)(bar_x + bwpx - 1), bar_y, lerp565(col, WHITE, 96));
        }
    }

    if (avg_w > min_w)
    {
        int32_t num = avg_w - min_w;
        if (num > den) num = den;
        if (num * 100 > den * 3)
        {
            uint16_t ay = (uint16_t)(gy + gh - 3 - (uint16_t)((uint32_t)num * (gh - 3) / (uint32_t)den) + 1);
            for (i = 0; i < gw; i += 6)
                LCD_Fill((uint16_t)(2 + i), ay, (uint16_t)(2 + i + 3), ay, WHITE);
        }
    }
}

void LCD_ShowIUW(int32_t I, int32_t U, int32_t W,
                 int32_t Imax, int32_t Umax, int32_t Wmax, int32_t Wavg,
                 int32_t charge_mAh, uint32_t uptime_s,
                 const int32_t *hist, uint8_t hist_len, uint32_t now_ms)
{
    static uint8_t  first  = 1;
    static uint8_t  page   = 0;
    static uint32_t t0     = 0;
    static uint32_t tg     = 0;
    static uint32_t tl     = 0;
    static uint8_t  live   = 0;
    static uint8_t  bat    = 0xFF;
    char b[12];

    if (first)
    {
        first = 0;
        t0 = now_ms;
        page = 0;
        ui_reset();
        ui_page1();
        live = 1;
        ui_live(live);
    }
    else
    {
        uint8_t np = (uint8_t)(((now_ms - t0) / 15000) & 1);
        if (np != page)
        {
            page = np;
            ui_reset();
            bat = 0xFF;
            if (page) ui_page2();
            else      ui_page1();
        }
    }

    if (page == 0)
    {
        uint8_t nb = (W > 500) ? 1u : 0u;
        if (nb != bat) { bat = nb; ui_battery(70, 238, bat); }

        if (ui_field(0, 8, 46, 154, 2, 1, ACCENT_V2, CARD_BG, (fmt_num(b, U, 4), b)))
            ui_pbar(10, 84, 146, (uint16_t)(U / 2500), ACCENT_V);

        if (ui_field(1, 8, 116, 154, 2, 1, ACCENT_A2, CARD_BG, (fmt_num(b, I, 4), b)))
            ui_pbar(10, 154, 146, (uint16_t)(I / 800), ACCENT_A);

        if (ui_field(2, 8, 186, 154, 2, 1, ACCENT_W2, CARD_BG, (fmt_num(b, W, 4), b)))
            ui_pbar(10, 224, 146, (uint16_t)(W / 18000), ACCENT_W);

        fmt_mah(b, charge_mAh);
        ui_field(3, 8, 254, 154, 2, 1, 0xFFE0, CARD_BG, b);
    }
    else
    {
        fmt_mah(b, charge_mAh);
        ui_field(4, 12, 50, 150, 2, 1, 0xFFE0, CARD_BG, b);

        fmt_num(b, Umax, 2); sapp(b, 'V');
        ui_field(5, 8, 132, 72, 1, 1, WHITE, CARD_BG, b);

        fmt_num(b, Imax, 3); sapp(b, 'A');
        ui_field(6, 90, 132, 72, 1, 1, WHITE, CARD_BG, b);

        fmt_num(b, Wmax, 2); sapp(b, 'W');
        ui_field(7, 8, 196, 72, 1, 1, WHITE, CARD_BG, b);

        fmt_num(b, Wavg, 2); sapp(b, 'W');
        ui_field(8, 90, 196, 72, 1, 1, WHITE, CARD_BG, b);

        fmt_time(b, uptime_s);
        ui_field(9, 84, 235, 74, 1, 2, WHITE, CARD_BG, b);
    }

    if ((uint32_t)(now_ms - tg) >= 500)
    {
        tg = now_ms;
        draw_graph(hist, hist_len, Wavg);
    }

    if ((uint32_t)(now_ms - tl) >= 500)
    {
        tl = now_ms;
        live = (uint8_t)!live;
        ui_live(live);
    }
}

/* ================= low-level hardware layer ================= */

void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    lcd_w_reg(0x2A);
    lcd_w_half_word((uint16_t)(x1 + TFT_COLUMN_OFFSET));
    lcd_w_half_word((uint16_t)(x2 + TFT_COLUMN_OFFSET));
    lcd_w_reg(0x2B);
    lcd_w_half_word((uint16_t)(y1 + TFT_LINE_OFFSET));
    lcd_w_half_word((uint16_t)(y2 + TFT_LINE_OFFSET));
    lcd_w_reg(0x2C);
}

void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t x, y;
    LCD_Address_Set(xsta, ysta, xend, yend);
    for (y = ysta; y < yend; y++)
    {
        for (x = xsta; x < xend; x++)
        {
            lcd_w_half_word(color);
        }
    }
}



void lcd_init(void)
{
    lcd_rst(0);
    Delay_ms(120);
    lcd_rst(1);
    Delay_ms(120);

    lcd_w_reg(0x11);
    Delay_ms(120);

    lcd_w_reg(0x3A); lcd_w_byte(0x05);
    lcd_w_reg(0xC5); lcd_w_byte(0x1A);
    lcd_w_reg(0x36); lcd_w_byte(LCD_MADCTL);
    lcd_w_reg(0xB2); lcd_w_byte(0x05); lcd_w_byte(0x05); lcd_w_byte(0x00);
    lcd_w_byte(0x33); lcd_w_byte(0x33);
    lcd_w_reg(0xB7); lcd_w_byte(0x05);
    lcd_w_reg(0xBB); lcd_w_byte(0x3F);
    lcd_w_reg(0xC0); lcd_w_byte(0x2C);
    lcd_w_reg(0xC2); lcd_w_byte(0x01);
    lcd_w_reg(0xC3); lcd_w_byte(0x0F);
    lcd_w_reg(0xC4); lcd_w_byte(0x20);
    lcd_w_reg(0xC6); lcd_w_byte(0x01);
    lcd_w_reg(0xD0); lcd_w_byte(0xA4); lcd_w_byte(0xA1);
    lcd_w_reg(0xE8); lcd_w_byte(0x03);
    lcd_w_reg(0xE9); lcd_w_byte(0x09); lcd_w_byte(0x09); lcd_w_byte(0x08);

    lcd_w_reg(0xE0);
    lcd_w_byte(0xD0); lcd_w_byte(0x05); lcd_w_byte(0x09); lcd_w_byte(0x09);
    lcd_w_byte(0x08); lcd_w_byte(0x14); lcd_w_byte(0x28); lcd_w_byte(0x33);
    lcd_w_byte(0x3F); lcd_w_byte(0x07); lcd_w_byte(0x13); lcd_w_byte(0x14);
    lcd_w_byte(0x28); lcd_w_byte(0x30);

    lcd_w_reg(0xE1);
    lcd_w_byte(0xD0); lcd_w_byte(0x05); lcd_w_byte(0x09); lcd_w_byte(0x09);
    lcd_w_byte(0x08); lcd_w_byte(0x03); lcd_w_byte(0x24); lcd_w_byte(0x32);
    lcd_w_byte(0x32); lcd_w_byte(0x3B); lcd_w_byte(0x14); lcd_w_byte(0x13);
    lcd_w_byte(0x28); lcd_w_byte(0x2F);

    lcd_w_reg(0x21);
    lcd_w_reg(0x29);
}

