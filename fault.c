#include <stdint.h>
#include "fault.h"
#include "crash_record.h"

#define SCB_CFSR   (*(volatile uint32_t *)0xE000ED28)
#define SCB_HFSR   (*(volatile uint32_t *)0xE000ED2C)
#define SCB_MMFAR  (*(volatile uint32_t *)0xE000ED34)
#define SCB_BFAR   (*(volatile uint32_t *)0xE000ED38)

volatile uint32_t hf_r0;
volatile uint32_t hf_r1;
volatile uint32_t hf_r2;
volatile uint32_t hf_r3;
volatile uint32_t hf_r12;
volatile uint32_t hf_lr;
volatile uint32_t hf_pc;
volatile uint32_t hf_xpsr;
volatile uint32_t hf_cfsr;
volatile uint32_t hf_hfsr;
volatile uint32_t hf_mmfar;
volatile uint32_t hf_bfar;
void uart_puts(const char *s);
void uart_puthex(uint32_t v);
static void print_fault_class(uint32_t cfsr)
{
    if (cfsr & 0x000000FF)
        uart_puts("Fault Class : MemManage Fault\n");

    if (cfsr & 0x0000FF00)
        uart_puts("Fault Class : Bus Fault\n");

    if (cfsr & 0xFFFF0000)
        uart_puts("Fault Class : Usage Fault\n");
}

static void print_bus_fault(uint32_t cfsr)
{
    uint8_t bfsr = (cfsr >> 8) & 0xFF;

    if (bfsr & (1 << 0))
        uart_puts("  IBUSERR   : Instruction bus error\n");

    if (bfsr & (1 << 1))
        uart_puts("  PRECISERR : Precise data bus error\n");

    if (bfsr & (1 << 2))
        uart_puts("  IMPRECIS  : Imprecise data bus error\n");

    if (bfsr & (1 << 3))
        uart_puts("  UNSTKERR  : Fault on exception unstacking\n");

    if (bfsr & (1 << 4))
        uart_puts("  STKERR    : Fault on exception stacking\n");

    if (bfsr & (1 << 7))
        uart_puts("  BFARVALID : BFAR holds valid address\n");
}

void hardfault_c(uint32_t *stacked)
{
    hf_r0   = stacked[0];
    hf_r1   = stacked[1];
    hf_r2   = stacked[2];
    hf_r3   = stacked[3];
    hf_r12  = stacked[4];
    hf_lr   = stacked[5];
    hf_pc   = stacked[6];
    hf_xpsr = stacked[7];
    hf_cfsr  = SCB_CFSR;
    hf_hfsr  = SCB_HFSR;
    hf_mmfar = SCB_MMFAR;
    hf_bfar  = SCB_BFAR;
    crash_record.magic = CRASH_MAGIC;
    crash_record.pc    = hf_pc;
    crash_record.lr    = hf_lr;
    crash_record.cfsr  = hf_cfsr;
    crash_record.hfsr  = hf_hfsr;
    crash_record.bfar  = hf_bfar;
    crash_record.mmfar = hf_mmfar;
    uart_puts("\n===== HARD FAULT =====\n");

    uart_puts("PC    : "); uart_puthex(hf_pc);    uart_puts("\n");
    uart_puts("LR    : "); uart_puthex(hf_lr);    uart_puts("\n");
    uart_puts("CFSR  : "); uart_puthex(hf_cfsr);  uart_puts("\n");
    uart_puts("HFSR  : "); uart_puthex(hf_hfsr);  uart_puts("\n");
    uart_puts("BFAR  : "); uart_puthex(hf_bfar);  uart_puts("\n");
    uart_puts("MMFAR : "); uart_puthex(hf_mmfar); uart_puts("\n");
    uart_puts("\n--- Fault Analysis ---\n");

    print_fault_class(hf_cfsr);

    if (hf_cfsr & 0x0000FF00)
    {
        print_bus_fault(hf_cfsr);

        if (hf_cfsr & (1 << 15))
        {
            uart_puts("  Fault Addr: ");
            uart_puthex(hf_bfar);
            uart_puts("\n");
        }
    }

    uart_puts("----------------------\n");
    while (1); 
}
