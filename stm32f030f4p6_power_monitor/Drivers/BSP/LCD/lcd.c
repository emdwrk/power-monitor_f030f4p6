#include "./BSP/LCD/lcd.h"

static uint16_t ui_line_buf[LCD_W];

/* ================= Unicode font (STZHONGS 16px bpp=1) ================= */

static const uint8_t font_bits[] = {
    0x0,
    0x70, 0x44, 0x42, 0x22, 0x21, 0x12, 0x8, 0x90,
    0x39, 0x38, 0x12, 0x20, 0x91, 0x8, 0x88, 0x84,
    0x44, 0x1c,
    0xf8,
    0xf0,
    0x3e, 0x31, 0x98, 0xd8, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0x43, 0x31, 0x8f, 0x0,
    0x33, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0x30, 0xcf,
    0xc0,
    0x3c, 0x46, 0x83, 0x83, 0x3, 0x6, 0x6, 0x8,
    0x31, 0x61, 0xff,
    0x3e, 0x47, 0x3, 0x6, 0x18, 0x6, 0x3, 0x83,
    0xc3, 0x46, 0x3c,
    0x6, 0x6, 0xe, 0x16, 0x26, 0x66, 0x46, 0xff,
    0x6, 0x6, 0xf,
    0x2, 0x7e, 0x40, 0x40, 0x7c, 0x6, 0x3, 0x3,
    0x83, 0xc3, 0xc6, 0x3c,
    0x1e, 0x30, 0x60, 0xdc, 0xe6, 0xc3, 0xc3, 0xc3,
    0xc3, 0x66, 0x3c,
    0x80, 0xff, 0x82, 0x2, 0x4, 0x8, 0x8, 0x18,
    0x10, 0x30, 0x30, 0x30,
    0x3e, 0x67, 0x43, 0x43, 0x26, 0x3e, 0xc3, 0x81,
    0x81, 0xc2, 0x3c,
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xe7, 0x7b, 0x3,
    0x6, 0xc, 0x78,
    0xf0, 0xf,
    0x4, 0x1, 0xc0, 0x38, 0x5, 0x1, 0x30, 0x26,
    0x4, 0x41, 0xfc, 0x21, 0x8c, 0x11, 0x3, 0x70,
    0xf0,
    0xff, 0x18, 0x76, 0xd, 0x83, 0x61, 0x9f, 0xc6,
    0x1d, 0x83, 0x60, 0xd8, 0x7f, 0xf0,
    0x1f, 0x46, 0x19, 0x81, 0x60, 0x2c, 0x1, 0x80,
    0x30, 0x6, 0x0, 0x60, 0x26, 0x18, 0x7c, 0x0,
    0xff, 0xc, 0x31, 0x83, 0x30, 0x36, 0x6, 0xc0,
    0xd8, 0x1b, 0x3, 0x60, 0xcc, 0x33, 0xfc, 0x0,
    0xff, 0x18, 0x66, 0x9, 0x90, 0x64, 0x1f, 0x6,
    0x41, 0x90, 0x60, 0x58, 0x6f, 0xf8,
    0xff, 0x30, 0x98, 0x2c, 0x86, 0x43, 0xe1, 0x90,
    0xc8, 0x60, 0x30, 0x3c, 0x0,
    0x1e, 0x86, 0x31, 0x83, 0x60, 0xc, 0x1, 0x83,
    0xf0, 0x36, 0x6, 0x60, 0xce, 0x38, 0x79, 0x0,
    0xf1, 0xec, 0x19, 0x83, 0x30, 0x66, 0xc, 0xff,
    0x98, 0x33, 0x6, 0x60, 0xcc, 0x1b, 0xc7, 0x80,
    0xf6, 0x66, 0x66, 0x66, 0x66, 0xf0,
    0x7, 0x81, 0x80, 0xc0, 0x60, 0x30, 0x18, 0xd,
    0x86, 0xc3, 0x63, 0xf, 0x0,
    0xf1, 0xcc, 0x31, 0x8c, 0x33, 0x6, 0xc0, 0xf8,
    0x1d, 0x83, 0x18, 0x61, 0x8c, 0x33, 0xcf, 0x80,
    0xf0, 0x30, 0x18, 0xc, 0x6, 0x3, 0x1, 0x80,
    0xc1, 0x60, 0xb1, 0xbf, 0xc0,
    0xe0, 0x7b, 0x83, 0x9c, 0x1c, 0xa1, 0x65, 0x8b,
    0x24, 0x99, 0x34, 0xc9, 0xa6, 0x46, 0x32, 0x31,
    0xb9, 0x9e,
    0xe1, 0xdc, 0x25, 0x89, 0x62, 0x4c, 0x91, 0x24,
    0x69, 0xe, 0x43, 0x90, 0x6e, 0x8,
    0x1f, 0x6, 0x31, 0x83, 0x60, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0x60, 0xc6, 0x30, 0x7c, 0x0,
    0xfe, 0x31, 0xd8, 0x6c, 0x36, 0x3b, 0xf9, 0x80,
    0xc0, 0x60, 0x30, 0x3c, 0x0,
    0x1f, 0x6, 0x31, 0x83, 0x60, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0x60, 0xc4, 0x10, 0x7c, 0x2,
    0x0, 0x64, 0x7, 0x0,
    0xff, 0xc, 0x39, 0x83, 0x30, 0x66, 0x1c, 0xfe,
    0x19, 0x3, 0x30, 0x63, 0xc, 0x33, 0xc3, 0x80,
    0x3e, 0x71, 0xb0, 0x58, 0x7, 0x0, 0xf0, 0xf,
    0x3, 0xc1, 0xf1, 0x97, 0x80,
    0x40, 0x9f, 0xec, 0xce, 0x31, 0xc, 0x3, 0x0,
    0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x78,
    0xf0, 0xec, 0x9, 0x81, 0x30, 0x26, 0x4, 0xc0,
    0x98, 0x13, 0x2, 0x60, 0x46, 0x10, 0x7c, 0x0,
    0xf0, 0xec, 0x8, 0x81, 0x18, 0x43, 0x8, 0x22,
    0x6, 0x40, 0x48, 0xe, 0x1, 0xc0, 0x10, 0x0,
    0xf3, 0x8e, 0xc2, 0x8, 0x86, 0x11, 0x8c, 0x43,
    0x18, 0x82, 0x51, 0x4, 0xb4, 0xd, 0x28, 0xc,
    0x50, 0x18, 0xc0, 0x30, 0x80,
    0x79, 0xe3, 0xc, 0x18, 0x81, 0x90, 0xe, 0x0,
    0x60, 0xb, 0x1, 0x18, 0x20, 0x86, 0xc, 0xf1,
    0xf0,
    0xf1, 0xd8, 0x22, 0x10, 0xc4, 0x1a, 0x7, 0x80,
    0xc0, 0x30, 0xc, 0x3, 0x1, 0xe0,
    0x7f, 0x90, 0x48, 0x30, 0x18, 0xc, 0x2, 0x1,
    0x80, 0xc1, 0x60, 0x90, 0x6f, 0xf8,
    0x10, 0xc0, 0x21, 0x10, 0xff, 0xf1, 0x4, 0x2,
    0x7f, 0xc, 0x86, 0x29, 0xfc, 0x52, 0x18, 0x27,
    0xf0, 0x48, 0x60, 0x9f, 0xc1, 0x21, 0x82, 0x43,
    0xc5, 0xff, 0x88, 0x0, 0x0,
    0x0, 0xe0, 0x13, 0x7, 0x8c, 0x85, 0xff, 0x10,
    0xc8, 0x43, 0x21, 0xc, 0x84, 0x22, 0x12, 0x88,
    0x76, 0x2f, 0x18, 0x90, 0xc2, 0x2, 0xf8, 0x10,
    0xc0, 0x80, 0x0,
    0x20, 0x8, 0x7f, 0xf8, 0x82, 0x1, 0x4, 0x2,
    0x8, 0x4, 0x12, 0xb, 0xfe, 0x10, 0x50, 0x20,
    0x90, 0x41, 0x30, 0x82, 0x42, 0x4, 0x27, 0xff,
    0xd0, 0x0, 0x0, 0x0, 0x0,
    0x10, 0xc0, 0x21, 0x80, 0x42, 0x0, 0x8f, 0xf1,
    0x10, 0x5f, 0xa0, 0x84, 0xa1, 0xa, 0x22, 0x10,
    0x44, 0x20, 0x8, 0x73, 0x97, 0x98, 0x24, 0x0,
    0x40, 0xf, 0x80, 0x4, 0x0,
    0x3, 0x0, 0x8, 0x0, 0x20, 0x0, 0x80, 0x2,
    0xb, 0xff, 0xf0, 0x30, 0x0, 0xc0, 0x2, 0x80,
    0x13, 0x0, 0x44, 0x2, 0x18, 0x10, 0x39, 0x80,
    0x70,
    0x0, 0x18, 0x7f, 0xf8, 0x4, 0x0, 0x88, 0x80,
    0x93, 0x1, 0xa4, 0x2, 0x50, 0x0, 0x86, 0xff,
    0xfc, 0x2, 0x0, 0x4, 0x0, 0x8, 0x0, 0x10,
    0x0, 0x20, 0x0, 0x40, 0x0,
    0x10, 0x20, 0x3f, 0xc0, 0x7f, 0x80, 0x81, 0x1,
    0xfe, 0x2, 0x1, 0x3f, 0xff, 0x11, 0x8, 0x3e,
    0xf8, 0x7d, 0x60, 0x89, 0xc1, 0x3b, 0xf, 0xa5,
    0x8, 0x71, 0xc0,
    0x0, 0xe0, 0x60, 0x88, 0x5f, 0xf8, 0x46, 0x4,
    0x99, 0x6, 0x61, 0x5, 0xbf, 0x9, 0xa8, 0x22,
    0x50, 0xc4, 0xa0, 0x89, 0x41, 0x32, 0xa2, 0x45,
    0x25, 0xb, 0xc0,
    0x3, 0x0, 0x2, 0x11, 0xff, 0xf1, 0x8, 0x1,
    0x6d, 0x83, 0xf2, 0x6, 0x88, 0xb, 0xec, 0x66,
    0x48, 0x82, 0x9b, 0xff, 0xf8, 0x8, 0x0, 0x10,
    0x0, 0x20, 0x0, 0x40, 0x0,
    0x8, 0x0, 0x80, 0x8, 0xf, 0xfe, 0x88, 0xc8,
    0x8c, 0xff, 0xc8, 0x8c, 0x88, 0xcf, 0xfc, 0x88,
    0xc0, 0x81, 0x8, 0x30, 0xff,
    0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00,
    0x04, 0x00, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20,
    0x04, 0x60, 0x04, 0x20, 0x7f, 0xe0, 0x04, 0x20,
    0x04, 0x20, 0x00, 0x60
};

static const uint8_t f_w[53] = {
    0,1,13,5,2,9,6,8,8,8,8,8,8,8,8,2,
    11,10,11,11,10,9,11,11,4,9,11,9,13,10,11,9,
    11,11,9,10,11,11,15,12,10,10,
    15,14,15,15,14,15,15,15,15,12,
    14
};
static const uint8_t f_h[53] = {
    0,1,11,1,2,11,11,11,11,11,12,11,12,11,11,8,
    12,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
    14,11,11,12,11,11,11,11,11,11,
    15,15,15,15,14,15,14,14,15,14,
    14
};
static const int8_t f_ox[53] = {
    0,0,1,1,2,1,3,1,1,1,1,1,1,1,1,2,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,0,1,0,0,0,0,1,
    1,1,1,1,1,1,1,0,0,3,
    1
};
static const int8_t f_oy[53] = {
    0,0,0,4,10,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    -3,0,0,0,0,0,0,0,0,0,
    -3,-3,-3,-2,-2,-3,-2,-2,-3,-2,
    -2
};
static const uint16_t f_bm[53] = {
    0,0,1,19,20,21,34,43,54,65,76,88,99,111,122,133,
    135,152,166,182,198,212,225,241,257,263,276,292,305,323,337,353,
    366,386,402,415,430,446,462,483,500,514,
    528,557,584,613,642,667,696,723,750,779,
    771
};
static const uint32_t f_uni[53] = {
    0, 0x20, 0x25, 0x2D, 0x2E,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A,
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B,
    0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56,
    0x57, 0x58, 0x59, 0x5A,
    0x503C, 0x529F, 0x538B, 0x5747, 0x5927, 0x5E73, 0x6700, 0x6D41, 0x7387, 0x7535,
    0x8BA1
};

static uint16_t font_find(uint32_t u)
{
    uint8_t i;
    for (i = 0; i < 53; i++)
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

void LCD_ShowChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg_color)
{
    (void)bg_color;
    gput(x, y, (uint8_t)ch, 1, color);
}

void LCD_ShowString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg_color)
{
    (void)bg_color;
    gputs(x, y, str, 1, color);
}

void LCD_ShowBigChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg_color)
{
    (void)bg_color;
    gput(x, y, (uint8_t)ch, 2, color);
}

void LCD_ShowBigString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg_color)
{
    (void)bg_color;
    gputs(x, y, str, 2, color);
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
        int16_t r = (int16_t)((c0 >> 11) & 0x1F) + (int16_t)(((int)((c1 >> 11) & 0x1F) - (int)((c0 >> 11) & 0x1F)) * t / 255);
        int16_t g = (int16_t)((c0 >> 5)  & 0x3F) + (int16_t)(((int)((c1 >> 5)  & 0x3F) - (int)((c0 >> 5)  & 0x3F)) * t / 255);
        int16_t b = (int16_t)(c0         & 0x1F) + (int16_t)(((int)(c1         & 0x1F) - (int)(c0         & 0x1F)) * t / 255);
        if (r < 0) r = 0; if (r > 31) r = 31;
        if (g < 0) g = 0; if (g > 63) g = 63;
        if (b < 0) b = 0; if (b > 31) b = 31;
        uint16_t col = (uint16_t)((r << 11) | (g << 5) | b);
        uint16_t px;
        uint32_t rows;
        uint16_t row;

        for (px = 0; px < w; px++) ui_line_buf[px] = col;
        rows = (uint16_t)(ye - y);
        LCD_Address_Set(x0, y, (uint16_t)(x0 + w - 1), (uint16_t)(ye - 1));
        for (row = 0; row < rows; row++)
            for (px = 0; px < w; px++)
                lcd_w_half_word(ui_line_buf[px]);
        y = ye;
    }
}

/* ================= LVGL-style Power Meter UI ================= */

void LCD_ShowIUW(float I, float U, float W,
                 float Imax, float Umax, float Wavg,
                 float energy_Wh,
                 const float *hist, uint8_t hist_len)
{
    static uint8_t  first   = 1;
    static uint16_t fcnt    = 0;
    static uint8_t  prev_ci = 0xFF;
    static uint16_t last_val_clrw[3];
    static long     last_val_raw[3];
    static long     last_sub_raw[3];
    static uint16_t last_sub_clrw[3];
    static long     last_energy_raw = -999999;
    static uint16_t last_energy_clrw = 0;
    static long     last_wavg_raw = -999999;
    static uint16_t last_wavg_clrw = 0;

    const uint16_t card_w  = 162;
    const uint16_t card_x  = 4;
    const uint16_t card_h  = 79;
    const uint16_t bar_w   = 4;
    const uint16_t cy[3]   = { 20, 102, 184 };
    const uint16_t info_y  = 266;
    const uint16_t graph_y = 295;
    const uint16_t graph_h = 25;

    static const uint16_t av[8] = { ACCENT_V, 0xFD00, ACCENT_V2, ORANGE,  LIME,  0x07E0, 0x04FF, 0xFFFF };
    static const uint16_t aa[8] = { ACCENT_A, 0x04FF, ACCENT_A2, 0x06BF, 0x20E0, 0xFC60, 0x04DF, 0xFFE0 };
    static const uint16_t aw[8] = { ACCENT_W, 0x97C0, ACCENT_W2, LIME,   0x07E0, 0xFD80, 0x06BF, 0xFFFF };
    const uint16_t *accent[3] = { av, aa, aw };
    uint8_t ci = (uint8_t)((fcnt / 10) & 0x07);

    if (first)
    {
        uint16_t i;

        LCD_FillGradBuf(0, 0, LCD_W, LCD_H, BG_DARK, BG_MID, 8);

        LCD_Fill(0, 0, LCD_W, 18, HEADER_BG);
        {
            const char *title = "\xE5\x8A\x9F\xE7\x8E\x87";
            uint16_t tw = gadvance(title, 1);
            gputs((LCD_W - tw) / 2, 3, title, 1, WHITE);
        }

        for (i = 0; i < 3; i++)
        {
            const char *label = (i == 0) ? "\xE7\x94\xB5\xE5\x8E\x8B"
                             : (i == 1) ? "\xE7\x94\xB5\xE6\xB5\x81"
                                       : "\xE5\x8A\x9F\xE7\x8E\x87";
            const char *unit = (i == 0) ? "V" : (i == 1) ? "A" : "W";
            uint16_t y  = cy[i];
            uint16_t ac = accent[i][0];

            LCD_Fill(card_x, y, (uint16_t)(card_x + card_w), (uint16_t)(y + card_h), CARD_BG);
            LCD_Fill(card_x, y, (uint16_t)(card_x + bar_w), (uint16_t)(y + card_h), ac);
            LCD_Fill(card_x, y, (uint16_t)(card_x + card_w), (uint16_t)(y + 1), ac);

            gputs((uint16_t)(card_x + bar_w + 5), (uint16_t)(y + 4), label, 1, TEXT_MID);
            {
                uint16_t uw = gadvance(unit, 1);
                gputs((uint16_t)(card_x + card_w - uw - 3), (uint16_t)(y + 4), unit, 1, TEXT_MID);
            }
        }

        {
            const char *info_label = "Energy";
            gputs(8, (uint16_t)(info_y + 2), info_label, 1, TEXT_DIM);
        }

        first = 0;
    }

    if (ci != prev_ci)
    {
        uint8_t i;
        prev_ci = ci;
        for (i = 0; i < 3; i++)
        {
            uint16_t y  = cy[i];
            uint16_t ac = accent[i][ci];
            LCD_Fill(card_x, (uint16_t)(y + 1), (uint16_t)(card_x + bar_w), (uint16_t)(y + card_h), ac);
            LCD_Fill(card_x, y, (uint16_t)(card_x + card_w), (uint16_t)(y + 1), ac);
        }
    }

    {
        uint8_t i;
        for (i = 0; i < 3; i++)
        {
            float    v   = (i == 0) ? U : (i == 1) ? I : W;
            uint16_t y   = cy[i];
            uint16_t ac  = accent[i][ci];
            char     buf[12];
            uint8_t  n = 0;
            long     ip, ipart, fpart;
            char     st[8];
            uint8_t  sn = 0;

            ip = (long)(v * 100.0f + (v >= 0 ? 0.5f : -0.5f));
            if (ip < 0) { buf[n++] = '-'; ip = -ip; }
            ipart = ip / 100;
            fpart = ip % 100;
            if (ipart == 0) { buf[n++] = '0'; }
            else { do { st[sn++] = (char)('0' + (ipart % 10)); ipart /= 10; } while (ipart > 0); }
            while (sn) buf[n++] = st[--sn];
            buf[n++] = '.';
            buf[n++] = (char)('0' + (fpart / 10));
            buf[n++] = (char)('0' + (fpart % 10));
            buf[n] = '\0';

            if (last_val_raw[i] != ip)
            {
                uint16_t nw = gadvance(buf, 2);
                uint16_t ox = (uint16_t)(card_x + bar_w + (card_w - bar_w - nw) / 2);
                uint16_t clr_w = (uint16_t)(last_val_clrw[i] > nw ? last_val_clrw[i] : nw);
                uint16_t clr_x = (uint16_t)(card_x + bar_w + (card_w - bar_w - clr_w) / 2);

                LCD_Fill(clr_x, (uint16_t)(y + 22), (uint16_t)(clr_x + clr_w), (uint16_t)(y + 55), CARD_BG);
                last_val_raw[i] = ip;
                last_val_clrw[i] = nw;
                gputs(ox, (uint16_t)(y + 28), buf, 2, ac);
            }

            {
                float     sv    = (i == 2) ? Wavg : (i == 0) ? Umax : Imax;
                char      sunit = (i == 0) ? 'V' : (i == 1) ? 'A' : 'W';
                const char *stag = (i == 2) ? "AVG:" : "MAX:";
                char      sbuf[16];
                uint8_t   k = 0;
                long      sip, sipart, sfpart;
                char      stmp[8];
                uint8_t   stn = 0;
                uint16_t  sx  = (uint16_t)(card_x + bar_w + 4);
                uint16_t  sy  = (uint16_t)(y + 60);

                sip = (long)(sv * 100.0f + (sv >= 0 ? 0.5f : -0.5f));

                if (last_sub_raw[i] != sip)
                {
                    while (*stag) sbuf[k++] = *stag++;
                    sbuf[k++] = ' ';
                    if (sip < 0) { sbuf[k++] = '-'; sip = -sip; }
                    sipart = sip / 100;
                    sfpart = sip % 100;
                    if (sipart == 0) { sbuf[k++] = '0'; }
                    else { do { stmp[stn++] = (char)('0' + (sipart % 10)); sipart /= 10; } while (sipart > 0); }
                    while (stn) sbuf[k++] = stmp[--stn];
                    sbuf[k++] = '.';
                    sbuf[k++] = (char)('0' + (sfpart / 10));
                    sbuf[k++] = (char)('0' + (sfpart % 10));
                    sbuf[k++] = sunit;
                    sbuf[k] = '\0';

                    {
                        uint16_t sub_w = gadvance(sbuf, 1);
                        uint16_t clr_w = (uint16_t)(last_sub_clrw[i] > sub_w ? last_sub_clrw[i] : sub_w);
                        LCD_Fill(sx, (uint16_t)(sy - 1), (uint16_t)(sx + clr_w + 2), (uint16_t)(sy + 13), CARD_BG);
                        last_sub_raw[i] = sip;
                        last_sub_clrw[i] = sub_w;
                    }
                    gputs(sx, sy, sbuf, 1, TEXT_MID);
                }
            }
        }
    }

    {
        char   ebuf[16];
        uint8_t en = 0;
        long   eip, eipart, efpart;
        char   est[8];
        uint8_t esn = 0;

        eip = (long)(energy_Wh * 1000.0f + 0.5f);
        eipart = eip / 1000;
        efpart = eip % 1000;

        if (eipart == 0) { ebuf[en++] = '0'; }
        else { do { est[esn++] = (char)('0' + (eipart % 10)); eipart /= 10; } while (eipart > 0); }
        while (esn) ebuf[en++] = est[--esn];
        ebuf[en++] = '.';
        ebuf[en++] = (char)('0' + (efpart / 100));
        ebuf[en++] = (char)('0' + ((efpart / 10) % 10));
        ebuf[en++] = (char)('0' + (efpart % 10));
        ebuf[en++] = 'W';
        ebuf[en++] = 'h';
        ebuf[en]   = '\0';

        if (last_energy_raw != eip)
        {
            uint16_t ew = gadvance(ebuf, 1);
            uint16_t ex = (uint16_t)(LCD_W - ew - 4);
            uint16_t ey = (uint16_t)(info_y + 2);
            uint16_t clr_w = (uint16_t)(last_energy_clrw > ew ? last_energy_clrw : ew);

            LCD_Fill((uint16_t)(ex - 2), ey, (uint16_t)(ex + clr_w + 2), (uint16_t)(ey + 13), BG_MID);
            last_energy_raw = eip;
            last_energy_clrw = ew;
            gputs(ex, ey, ebuf, 1, ACCENT_W2);
        }
    }

    {
        float wavg = Wavg;
        long  wavg_raw = (long)(wavg * 100.0f + 0.5f);

        if (last_wavg_raw != wavg_raw)
        {
            char    wbuf[16];
            uint8_t wn = 0;
            long    wip, wipart, wfpart;
            char    wst[8];
            uint8_t wsn = 0;
            uint16_t sx = (uint16_t)(card_x + bar_w + 4);
            uint16_t sy = (uint16_t)(cy[2] + 60);

            wip = wavg_raw;
            if (wip < 0) { wbuf[wn++] = '-'; wip = -wip; }
            wipart = wip / 100;
            wfpart = wip % 100;
            if (wipart == 0) { wbuf[wn++] = '0'; }
            else { do { wst[wsn++] = (char)('0' + (wipart % 10)); wipart /= 10; } while (wipart > 0); }
            while (wsn) wbuf[wn++] = wst[--wsn];
            wbuf[wn++] = '.';
            wbuf[wn++] = (char)('0' + (wfpart / 10));
            wbuf[wn++] = (char)('0' + (wfpart % 10));
            wbuf[wn++] = 'W';
            wbuf[wn]   = '\0';

            {
                uint16_t sub_w = gadvance(wbuf, 1);
                uint16_t clr_w = (uint16_t)(last_wavg_clrw > sub_w ? last_wavg_clrw : sub_w);
                LCD_Fill(sx + 40, (uint16_t)(sy - 1), (uint16_t)(sx + 40 + clr_w + 2), (uint16_t)(sy + 13), CARD_BG);
                last_wavg_raw = wavg_raw;
                last_wavg_clrw = sub_w;
            }
            gputs((uint16_t)(sx + 40), sy, wbuf, 1, TEXT_MID);
        }
    }

    if ((fcnt & 3) == 0 && hist_len > 0 && hist)
    {
        uint8_t i;
        uint16_t gy = graph_y;
        uint16_t gh = graph_h;
        uint16_t gw = (uint16_t)(LCD_W - 4);
        float    max_w = 0.0f;
        float    min_w = 9999.0f;

        for (i = 0; i < hist_len; i++)
        {
            float hw = hist[i];
            if (hw > max_w) max_w = hw;
            if (hw < min_w) min_w = hw;
        }
        if (max_w < 0.5f) max_w = 0.5f;
        if (max_w - min_w < 0.1f) max_w = min_w + 0.1f;

        LCD_Fill(2, gy, (uint16_t)(2 + gw), (uint16_t)(gy + gh), BG_DARK);
        LCD_Fill(2, (uint16_t)(gy + gh - 1), (uint16_t)(2 + gw), (uint16_t)(gy + gh), GRAY);

        {
            uint16_t bar_w_px = (uint16_t)(gw / hist_len);
            if (bar_w_px < 2) bar_w_px = 2;

            for (i = 0; i < hist_len; i++)
            {
                uint16_t h;
                uint16_t bar_x, bar_h, bar_y;
                uint16_t col;
                float norm;

                h = (uint16_t)(gw * i / hist_len);
                if (h >= gw) continue;
                norm = (hist[i] - min_w) / (max_w - min_w);
                if (norm < 0.0f) norm = 0.0f;
                if (norm > 1.0f) norm = 1.0f;

                bar_x = (uint16_t)(2 + gw * i / hist_len);
                bar_h = (uint16_t)((gh - 3) * norm);
                bar_y = (uint16_t)(gy + gh - 3 - bar_h + 1);

                if (norm < 0.33f)      col = 0x06BF;
                else if (norm < 0.66f) col = ACCENT_W;
                else                    col = ACCENT_V;

                if (bar_h > 0)
                    LCD_Fill(bar_x, bar_y, (uint16_t)(bar_x + bar_w_px - 1), (uint16_t)(bar_y + bar_h), col);
            }
        }
    }

    fcnt++;
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

void lcd_test_demo(void)
{
    LCD_Fill(0, 0, LCD_W, LCD_H, RED);
    Delay_ms(200);
    LCD_Fill(0, 0, LCD_W, LCD_H, GREEN);
    Delay_ms(200);
    LCD_Fill(0, 0, LCD_W, LCD_H, BLUE);
    Delay_ms(200);
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
}