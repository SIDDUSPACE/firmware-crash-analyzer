#include <stdint.h>
#include "crash_record.h"
void uart_init(void);
void uart_puts(const char *s);
void uart_puthex(uint32_t v);
static void trigger_fault(void);
#define SCB_SHCSR (*(volatile uint32_t *)0xE000ED24)

int main(void)
{
    SCB_SHCSR |= (1 << 16) | (1 << 17) | (1 << 18);
    uart_init();
    if (crash_record.magic == CRASH_MAGIC)
    {
        uart_puts("\n*** PREVIOUS CRASH DETECTED ***\n");

        uart_puts("PC    : "); uart_puthex(crash_record.pc);    uart_puts("\n");
        uart_puts("LR    : "); uart_puthex(crash_record.lr);    uart_puts("\n");
        uart_puts("CFSR  : "); uart_puthex(crash_record.cfsr);  uart_puts("\n");
        uart_puts("HFSR  : "); uart_puthex(crash_record.hfsr);  uart_puts("\n");
        uart_puts("BFAR  : "); uart_puthex(crash_record.bfar);  uart_puts("\n");
        uart_puts("MMFAR : "); uart_puthex(crash_record.mmfar); uart_puts("\n");

        uart_puts("*****************************\n");
        crash_record.magic = 0;
    }
    for (volatile int i = 0; i < 100000; i++);
    trigger_fault();

    while (1);
}

static void trigger_fault(void)
{
    __asm volatile (".word 0xFFFFFFFF");
}
