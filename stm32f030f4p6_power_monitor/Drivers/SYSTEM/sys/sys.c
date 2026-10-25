#include "./SYSTEM/sys/sys.h"
#include "stm32f0xx_hal.h"

void SystemClock_Init(void)
{
    RCC_OscInitTypeDef RCC_OSC_InitStructure = {0};
    RCC_ClkInitTypeDef RCC_Clk_InitStructure = {0};

    RCC_OSC_InitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OSC_InitStructure.HSEState = RCC_HSE_ON;
    RCC_OSC_InitStructure.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OSC_InitStructure.PLL.PLLState = RCC_PLL_ON;
    RCC_OSC_InitStructure.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OSC_InitStructure.PLL.PLLMUL = RCC_PLL_MUL6;
    RCC_OSC_InitStructure.PLL.PREDIV = RCC_PREDIV_DIV1;

    RCC_Clk_InitStructure.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_Clk_InitStructure.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_Clk_InitStructure.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_Clk_InitStructure.APB1CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_OscConfig(&RCC_OSC_InitStructure);
    
    
    if(HAL_RCC_ClockConfig(&RCC_Clk_InitStructure, FLASH_LATENCY_1) != HAL_OK) while(1);
    
    SystemCoreClockUpdate();
}
