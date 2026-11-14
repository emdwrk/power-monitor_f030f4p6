#ifndef __LCD_H__
#define __LCD_H__   

#include "./BSP/SPI/spi.h"
#include "SYSTEM/sys/sys.h"
#include "SYSTEM/Delay/Delay.h"

#define USE_HORIZONTAL 0

#if USE_HORIZONTAL==0 || USE_HORIZONTAL==2
#define TFT_COLUMN_OFFSET 35
#define TFT_LINE_OFFSET   0
#define LCD_W 170
#define LCD_H 320
#else
#define TFT_COLUMN_OFFSET 0
#define TFT_LINE_OFFSET   0
#define LCD_W 320
#define LCD_H 170
#endif

/* MADCTL: vert=0x00; horz MX+MV=0x60; horz MV+MY=0xA0 */
#define LCD_MADCTL 0x00

#define RED    0xF800
#define GREEN  0x07E0
#define BLUE   0x001F
#define WHITE  0xFFFF
#define BLACK  0x0000
#define YELLOW 0xFFE0
#define CYAN   0x07FF

/* extended palette */
#define ORANGE   0xFD20
#define LIME     0x9FE0
#define GRAY     0x8410
#define DARKGRAY 0x4208
#define NAVY     0x000F

/* LVGL-like palette */
#define BG_DARK    0x0863
#define BG_MID     0x18E5
#define CARD_BG    0x294A
#define CARD_SHAD  0x0841
#define ACCENT_V   0xFC60
#define ACCENT_V2  0xFD80
#define ACCENT_A   0x073F
#define ACCENT_A2  0x059F
#define ACCENT_W   0x87E0
#define ACCENT_W2  0xAF60
#define TEXT_DIM   0x9CD3
#define TEXT_MID   0xCE59
#define HEADER_BG  0x02BF

/* sky background / clouds */
#define SKY_TOP    0x03DF
#define SKY_MID    0x3E9F
#define SKY_BOT    0xD7FF
#define CLOUD      0xFFFF
#define CLOUD_SH   0xCEDF
#define GRAPH_BG   0x08A3

/* landscape / nature theme */
#define SKY_DAWN   0x0863
#define SUNRISE    0xFD40
#define MT_FAR     0x3D6B
#define MT_NEAR    0x1A63
#define MT_SHADE   0x1121
#define WATER      0x03BF
#define WATER_MID  0x057F
#define WATER_HI   0xBFDF

void lcd_init(void);
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_ShowIUW(int32_t I, int32_t U, int32_t W,
                 int32_t Imax, int32_t Umax, int32_t Wmax, int32_t Wavg,
                 int32_t charge_mAh, uint32_t uptime_s,
                 const int32_t *hist, uint8_t hist_len, uint32_t now_ms);
void LCD_BootAnim(void);


#endif
