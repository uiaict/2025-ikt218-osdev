#include <libc/pit.h>
#include <libc/io.h>
#include <libc/isr.h>
#include <libc/terminal.h>

static volatile uint32_t tick = 0;

// Viktig: Handler må ta inn `registers_t`
void pit_handler(registers_t regs)
{
    tick++;

    char buffer[16];
    itoa(tick, buffer, 10);
    outb(0x20, 0x20); // Send EOI
}

uint32_t get_current_tick()
{
    return tick;
}

void init_pit()
{
    terminal_write("Initializing PIT...\n");

    // Konfigurer PIT: Channel 0, mode 3 (square wave), low/high byte access
    outb(PIT_CMD_PORT, 0x36);
    outb(PIT_CHANNEL0_PORT, (uint8_t)(DIVIDER & 0xFF));        // Low byte
    outb(PIT_CHANNEL0_PORT, (uint8_t)((DIVIDER >> 8) & 0xFF)); // High byte

    // Registrer PIT handler på IRQ0 (interrupt 32)
    register_interrupt_handler(32, pit_handler);
}

void sleep_busy(uint32_t milliseconds)
{
    uint32_t start = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end = start + ticks_to_wait;

    if (end < start) // Overflow
    {
        while (get_current_tick() >= start)
        {
        }
        while (get_current_tick() < end)
        {
        }
    }
    else
    {
        while (get_current_tick() < end)
        {
        }
    }
}

void sleep_interrupt(uint32_t milliseconds)
{
    uint32_t start = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end = start + ticks_to_wait;

    if (end < start) // Overflow
    {
        while (get_current_tick() >= start)
        {
            __asm__ __volatile__("sti\nhlt");
        }
        while (get_current_tick() < end)
        {
            __asm__ __volatile__("sti\nhlt");
        }
    }
    else
    {
        while (get_current_tick() < end)
        {
            __asm__ __volatile__("sti\nhlt");
        }
    }
}
