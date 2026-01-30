#include <stdint.h>

/* RCC */
#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR   (*(volatile uint32_t*)0x40023840)

/* GPIOA */
#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_AFRL    (*(volatile uint32_t*)0x40020020)

/* USART2 */
#define USART2_SR     (*(volatile uint32_t*)0x40004400)
#define USART2_DR     (*(volatile uint32_t*)0x40004404)
#define USART2_BRR    (*(volatile uint32_t*)0x40004408)
#define USART2_CR1    (*(volatile uint32_t*)0x4000440C)

/* Initialize UART2 (PA2 TX, 9600 baud @ 16 MHz) */
void uart_init(void)
{
    /* Enable clocks */
    RCC_AHB1ENR |= (1 << 0);    /* GPIOA */
    RCC_APB1ENR |= (1 << 17);   /* USART2 */

    /* PA2 → Alternate Function */
    GPIOA_MODER &= ~(3 << (2 * 2));
    GPIOA_MODER |=  (2 << (2 * 2));

    /* AF7 for USART2 */
    GPIOA_AFRL &= ~(0xF << (4 * 2));
    GPIOA_AFRL |=  (7   << (4 * 2));

    /* Baud rate: 9600 @ 16 MHz */
    USART2_BRR = 0x0683;

    /* Enable USART + TX */
    USART2_CR1 = (1 << 13) | (1 << 3);
}

/* Send one character */
void uart_putc(char c)
{
    while (!(USART2_SR & (1 << 7))); /* TXE */
    USART2_DR = c;
}

/* Send string */
void uart_puts(const char *s)
{
    while (*s)
        uart_putc(*s++);
}

/* Print 32-bit hex value */
void uart_puthex(uint32_t v)
{
    static const char hex[] = "0123456789ABCDEF";

    uart_puts("0x");
    for (int i = 28; i >= 0; i -= 4)
        uart_putc(hex[(v >> i) & 0xF]);
}
