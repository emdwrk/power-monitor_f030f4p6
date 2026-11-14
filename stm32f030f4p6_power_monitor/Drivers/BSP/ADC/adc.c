#include "./BSP/ADC/adc.h"


/* PA1 CURRENT  PA2 VOLTAGE */
DMA_HandleTypeDef hdma1 = {0};
ADC_HandleTypeDef hadc1 = {0};

volatile uint16_t adc_dma_buff[2];

void adc_init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();
    hdma1.Instance = DMA1_Channel1;
    hdma1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma1.Init.MemInc = DMA_MINC_ENABLE;
    hdma1.Init.Mode = DMA_CIRCULAR;
    hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma1.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&hdma1);
    __HAL_LINKDMA(&hadc1,DMA_Handle,hdma1);

    hadc1.Instance = ADC1;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4; 
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.LowPowerAutoPowerOff = DISABLE;
    hadc1.Init.LowPowerAutoWait = DISABLE;
    hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    hadc1.Init.SamplingTimeCommon = ADC_SAMPLETIME_28CYCLES_5;
    hadc1.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
    HAL_ADC_Init(&hadc1);

    ADC_ChannelConfTypeDef adc_channel_cfg = {0};
    adc_channel_cfg.Channel = ADC_CHANNEL_1;
    adc_channel_cfg.Rank = ADC_RANK_CHANNEL_NUMBER;
    adc_channel_cfg.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1,&adc_channel_cfg);

    adc_channel_cfg.Channel = ADC_CHANNEL_2;
    HAL_ADC_ConfigChannel(&hadc1,&adc_channel_cfg);
    
    Delay_ms(100);

    HAL_ADCEx_Calibration_Start(&hadc1);

    HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_dma_buff,2);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
        GPIO_InitTypeDef gpio_init_struct = {0};

        __HAL_RCC_ADC1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        gpio_init_struct.Pin = GPIO_PIN_1 | GPIO_PIN_2;
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA,&gpio_init_struct);
    }
}

/*
 * 14-bit oversampling via 16x accumulate >> 2.
 *
 * Double-read with re-check: read buff[0], read buff[1], then read buff[0]
 * again.  If buff[0] changed the buff[1] value may belong to a different
 * scan pair; retry until buff[0] is stable.
 *
 * DMA writes buff[0] BEFORE buff[1] in each scan, so a stable buff[0]
 * guarantees buff[1] is also from the same scan.
 */
static uint8_t adc_16bit_sample(uint16_t *result_data)
{
    uint8_t  i;
    uint32_t sum0 = 0, sum1 = 0;

    for(i = 0; i < 255; i++)
    {
        uint16_t v0, v1, v0_check;

        do {
            v0       = adc_dma_buff[0];
            v1       = adc_dma_buff[1];
            v0_check = adc_dma_buff[0];
        } while (v0 != v0_check);

        sum0 += v0;
        sum1 += v1;
    }

    result_data[0] = (uint16_t)(sum0 >> 4);
    result_data[1] = (uint16_t)(sum1 >> 4);
    return 0;
}


uint8_t adc_convert_to_IandV(int32_t *I, int32_t *V, int32_t *W)
{
    uint16_t result_data[2] = {0};
    uint32_t r0, r1, i, v, ihi, ilo, vhi, vlo;

    adc_16bit_sample(result_data);

    r0 = result_data[0];
    r1 = result_data[1];

    /* I(A) = r0 * 3.29 / (65536 * 0.4)  ->  I*10000 = r0 * 82250 / 65536 */
    i = r0 + ((r0 * 16714u + 32768u) >> 16);
    /* V(V) = r1 * 3.29 / 65536 * 34500/5100  ->  V*10000 = r1 * 222559 / 65536 */
    v = r1 * 3u + ((r1 * 25951u + 32768u) >> 16);

    ihi = i / 10000u;  ilo = i % 10000u;
    vhi = v / 10000u;  vlo = v % 10000u;

    *I = (int32_t)i;
    *V = (int32_t)v;
    *W = (int32_t)(ihi * vhi * 10000u + ihi * vlo + ilo * vhi + (ilo * vlo) / 10000u);
    return 0;
}
