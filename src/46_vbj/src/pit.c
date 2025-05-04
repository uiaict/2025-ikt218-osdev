#include "pit.h"
#include "terminal.h"
#include "isr.h"
#include "libc/stdint.h"
#include "descTables.h"
#include "global.h"
#
static volatile uint32_t tick = 0;


uint32_t get_tick()
{
    return tick;
}

void pit_handler()
{
    tick++;
   /*
   if (tick % 100 == 0) {
        printf("Tick: %d\n", tick);  
    }*/
    outb(PIC1_CMD_PORT, PIC_EOI);
}

void init_pit() {
    //printf("Initializing PIT...\n");  

    uint16_t divisor = PIT_BASE_FREQUENCY / TARGET_FREQUENCY;

    outb(PIT_CMD_PORT, 0x36);

    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);   // Low byte
    outb(PIT_CHANNEL0_PORT, divisor >> 8);     // High byte

    register_interrupt_handler(32, pit_handler);
}


// Sleep using interrupts
void sleep_interrupt(uint32_t milliseconds)
{
    uint32_t current_ticks = get_tick();
    uint32_t tics_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_ticks = current_ticks + tics_to_wait;

    
    //printf("Start Tick: %d, End Tick: %d, Duration (ms): %d\n", current_ticks, end_ticks, milliseconds);

    while (current_ticks < end_ticks)
    {
        asm volatile("sti");
        asm volatile("hlt");
        current_ticks = get_tick();
    }
    //printf("End Tick: %d\n", current_ticks);
}

// Sleep using busy waiting
void sleep_busy(uint32_t milliseconds)
{
    uint32_t start_tick = get_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;
    while(elapsed_ticks < ticks_to_wait)
    {
        while (get_tick() <= start_tick + elapsed_ticks)
        {
            /* BUSY WAIT - do nothing */
           // printf("Waiting... elapsed_ticks = %d\n", elapsed_ticks);
        }
        elapsed_ticks++;
    }
}