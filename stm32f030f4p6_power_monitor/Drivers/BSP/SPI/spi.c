#include "./BSP/SPI/spi.h"


/*
 * Hardware SPI1 initialisation.
 * PA5=SCK(AF0), PA7=MOSI(AF0) at 12MHz, Mode 0 (CPOL=0,CPHA=0).
 * PA3=RS, PA4=CS, PA6=RST kept as push-pull GPIO (PA6 is also MISO
 * but we run SPI1 in tx-only master mode so MISO is not needed).
 */
void spi_init(void)
{
    GPIO_InitTypeDef gpio = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();

    /* PA3=RS, PA4=CS, PA6=RST: plain GPIO output */
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Pin   = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6;
    HAL_GPIO_Init(GPIOA, &gpio);

    /* PA5=SCK, PA7=MOSI: AF0 (SPI1) */
    gpio.Mode  = GPIO_MODE_AF_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Pin   = GPIO_PIN_5 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &gpio);

    lcd_rs(1);
    lcd_cs(1);
    lcd_rst(1);

    /* SPI1 config: Master, 8-bit, MSB first, fPCLK/4 = 12 MHz */
    SPI1->CR1 = SPI_CR1_MSTR        /* master mode                */
              | SPI_CR1_SSM         /* software slave management  */
              | SPI_CR1_SSI         /* internal NSS high          */
            //   | SPI_CR1_BR       /* BR=001 -> fPCLK/4 = 12MHz  */
              | SPI_CR1_SPE;        /* enable                     */

}

/*
 * Send one byte via SPI1.
 * Sequence: assert CS -> wait TXE -> write DR -> wait BSY=0 -> deassert CS.
 * BSY poll guarantees the last SCK edge finished before CS rises.
 */
uint8_t lcd_w_byte(uint8_t byte)
{
    lcd_cs(0);

    while (!(SPI1->SR & SPI_SR_TXE));
    *(volatile uint8_t *)&SPI1->DR = byte;
    while (!(SPI1->SR & SPI_SR_TXE));
    while (SPI1->SR & SPI_SR_BSY);

    lcd_cs(1);
    return 0;
}

/*
 * Send 16-bit half-word (MSB first) in two 8-bit chunks.
 */
void lcd_w_half_word(uint16_t half_word)
{
    lcd_cs(0);

    while (!(SPI1->SR & SPI_SR_TXE));
    *(volatile uint8_t *)&SPI1->DR = (uint8_t)(half_word >> 8);
    while (!(SPI1->SR & SPI_SR_TXE));
    while (SPI1->SR & SPI_SR_BSY);

    *(volatile uint8_t *)&SPI1->DR = (uint8_t)(half_word);
    while (!(SPI1->SR & SPI_SR_TXE));
    while (SPI1->SR & SPI_SR_BSY);

    lcd_cs(1);
}

/*
 * Write command register: DC=0 send command, DC=1 for data mode.
 */
uint8_t lcd_w_reg(uint8_t reg)
{
    lcd_rs(0);
    lcd_cs(0);

    while (!(SPI1->SR & SPI_SR_TXE));
    *(volatile uint8_t *)&SPI1->DR = reg;
    while (!(SPI1->SR & SPI_SR_TXE));
    while (SPI1->SR & SPI_SR_BSY);

    lcd_cs(1);
    lcd_rs(1);
    return 0;
}