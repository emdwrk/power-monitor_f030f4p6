#include "./BSP/SWITCH/switch.h"

void switch_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio_init_struct.Pin = GPIO_PIN_0;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA,&gpio_init_struct);
}

void switch_set(uint8_t state)
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,(GPIO_PinState)state);
}

