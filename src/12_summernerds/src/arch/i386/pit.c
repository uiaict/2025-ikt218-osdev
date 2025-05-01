#include "i386/descriptorTables.h"
#include "i386/interruptRegister.h"
#include <kernel/pit.h>
#include <libc/stdint.h>
#include <libc/stdio.h>
#include "common.h"

#define BRUTE_FORCE_CONSTANT 140000

static uint32_t pit_ticks = 0;

void pit_callback(registers_t *regs, void *ctx)
{
    pit_ticks++;
    outb(PIC1_CMD_PORT, PIC_EOI);
}

void init_pit()
{
    register_irq_handler(IRQ0, pit_callback, NULL);

    uint32_t divisor = PIT_BASE_FREQUENCY / TARGET_FREQUENCY;

    outb(PIT_CMD_PORT, 0x36);
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF));

    pit_tickets = 0;
    print("Initialize PIT with %d Hz\n", TARGET_FREQUENCY);
}

// A safe way to access pit_ticks
uint32_t get_current_tick()
{
    return pit_ticks;
}

// Sleeps with high processor power
void sleep_busy(uint32_t wait_ms)
{
    uint32_t start_tick = get_current_tick();
    uint32_t wait_ticks = wait_ms;
    uint32_t waited_ticks = 0;
    while ((waited_ticks) < wait_ticks)
    {
        while (get_current_tick() == start_tick + waited_ticks)
            ;
        waited_ticks++;
    }
}

// Sleeps with low processor power
void sleep_interrupt(uint32_t wait_ticks)
{
    uint32_t end_tick = get_current_tick() + wait_ticks;
    while (get_current_tick() < end_tick)
    {
        asm volatile("sti");
        asm volatile("hlt");
    }
}
