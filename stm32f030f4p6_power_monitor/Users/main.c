#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/Delay/Delay.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/SWITCH/switch.h"
#include "./BSP/ADC/adc.h"

/* All measurement values are fixed-point: value * 10000 (4 decimals). */
int32_t I = 0;
int32_t V = 0;
int32_t W = 0;

#define HIST_LEN  120
#define HIST_MS   250u
#define THR_W     500

static int32_t g_hist_w[HIST_LEN];
static uint16_t g_hpos = 0;
static int32_t g_charge_mAh = 0;
static int32_t g_peak_w = 0;

int main(void)
{
    int32_t Vmax = 0, Imax = 0, Wavg = 0;
    int32_t Is = 0, Vs = 0, Ws = 0;
    uint32_t frame = 0;
    uint32_t t_last, t_now, t_hist = 0;

    HAL_Init();
    SystemClock_Init();
    Delay_Init(48);
    spi_init();
    lcd_init();
    switch_init();
    switch_set(1);
    LCD_BootAnim();
    adc_init();

    t_last = HAL_GetTick();

    while (1)
    {
        int32_t ri, rv, rw;
        uint32_t dt_ms;

        t_now = HAL_GetTick();
        dt_ms = t_now - t_last;
        t_last = t_now;
        if (dt_ms > 500u) dt_ms = 500u;

        adc_convert_to_IandV(&ri, &rv, &rw);

        if (frame == 0) { Is = ri; Vs = rv; Ws = rw; }
        else
        {
            Is = (9 * Is + ri) / 10;
            Vs = (9 * Vs + rv) / 10;
            Ws = (9 * Ws + rw) / 10;
        }

        I = Is; V = Vs; W = Ws;

        if (I > Imax) Imax = I;
        if (V > Vmax) Vmax = V;
        if (W > g_peak_w) g_peak_w = W;

        if (frame == 0) Wavg = W;
        else            Wavg = (49 * Wavg + W) / 50;

        if (W > THR_W) g_charge_mAh += I * dt_ms / 3600u;

        if ((uint32_t)(t_now - t_hist) >= HIST_MS)
        {
            t_hist = t_now;
            g_hist_w[g_hpos] = W;
            g_hpos = (g_hpos + 1) % HIST_LEN;
        }

        LCD_ShowIUW(I, V, W, Imax, Vmax, g_peak_w, Wavg,
                    g_charge_mAh, t_now / 1000u,
                    g_hist_w, HIST_LEN, t_now);

        frame++;
    }
}
