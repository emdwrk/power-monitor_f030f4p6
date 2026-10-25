#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/Delay/Delay.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/SWITCH/switch.h"
#include "./BSP/ADC/adc.h"

float I = 0.0f;
float V = 0.0f;
float W = 0.0f;

#define HIST_LEN  120
#define EMA_A     0.10f

static float g_hist_w[HIST_LEN];
static float g_hist_v[HIST_LEN];
static float g_hist_i[HIST_LEN];
static uint16_t g_hpos = 0;
static float g_energy_Ws = 0.0f;
static float g_peak_w = 0.0f;
static float g_min_w  = 9999.0f;

int main(void)
{
    float Vmax = 0.0f, Imax = 0.0f, Wavg = 0.0f;
    float Is = 0.0f, Vs = 0.0f, Ws = 0.0f;
    uint32_t frame = 0;

    HAL_Init();
    SystemClock_Init();
    Delay_Init(48);
    spi_init();
    lcd_init();
    switch_init();
    switch_set(1);
    adc_init();

    while(1)
    {
        float ri, rv, rw;
        adc_convert_to_IandV(&ri, &rv, &rw);

        if (frame == 0) { Is = ri; Vs = rv; Ws = rw; }
        else {
            Is = (1.0f - EMA_A) * Is + EMA_A * ri;
            Vs = (1.0f - EMA_A) * Vs + EMA_A * rv;
            Ws = (1.0f - EMA_A) * Ws + EMA_A * rw;
        }

        I = Is; V = Vs; W = Ws;

        if (I > Imax) Imax = I;
        if (V > Vmax) Vmax = V;

        if (frame == 0) Wavg = W;
        else            Wavg = 0.98f * Wavg + 0.02f * W;

        g_energy_Ws += W * 0.125f;

        if ((frame & 3) == 0) {
            g_hist_w[g_hpos] = W;
            g_hist_v[g_hpos] = V;
            g_hist_i[g_hpos] = I;
            g_hpos = (g_hpos + 1) % HIST_LEN;
        }

        if (W > g_peak_w) g_peak_w = W;
        if (W < g_min_w && W > 0.001f) g_min_w = W;
        if ((frame & 2047) == 0) { g_peak_w *= 0.5f; g_min_w = 9999.0f; }

        LCD_ShowIUW(I, V, W, Imax, Vmax, Wavg,
                    g_energy_Ws / 3600.0f,
                    g_hist_w, HIST_LEN);

        frame++;

        if ((frame & 127) == 0) {
            Imax *= 0.92f;
            Vmax *= 0.92f;
        }
    }
}