#ifndef __SPI_H__
#define __SPI_H__

#include "SYSTEM/sys/sys.h"
#include "SYSTEM/Delay/Delay.h"

/* GPIO handles: RS, CS, RST stay as GPIO; SCK+MOSI now on SPI1 peripheral */
#define lcd_rs(X)   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,(GPIO_PinState)(X ? 1 : 0))
#define lcd_cs(X)   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,(GPIO_PinState)(X ? 1 : 0))
#define lcd_rst(X)  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,(GPIO_PinState)(X ? 1 : 0))

/* SCK=PA5(AF0), MOSI=PA7(AF0) -- driven by SPI1 HW, no more bit-bang macros */

void spi_init(void);
uint8_t lcd_w_byte(uint8_t byte);
uint8_t lcd_w_reg(uint8_t reg);
void lcd_w_half_word(uint16_t half_word);

#endif
