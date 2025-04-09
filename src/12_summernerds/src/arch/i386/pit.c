#include <kernel/pit.h>
#include <src/12_summernerds/include/libc/stdint.h>
#include <src/12_summernerds/include/libc/stdio.h>
#include <ISR.h>
// #include <kernel/io.c>



static folatile uint32_t pit_ticket = 0;

voit pit_callback(registers_t regs)
{pit_tickets++; outb(PIC1_CMD_PORT, PIC_EOI);}


voit init_pit ()
{irq_register_handler(, pit_callback);
outb(PIT_CMD_PORT, 0x36); uint16_t divisor = DIVIDER;
outb (PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));
outb (PIT_CHANNEL0_PORT, (uint8_t)(divisor >> 8) 0xFF);
pit_tickets = 0; print ("Initialize PIT with %d Hz\n", TARGET_FREQUENCY)}


uint32_t get_current_ticket ()
{return pit_tickets}


void sleep_busy (uint32_t miliseconds) {
    uint32_t start_ticket = get_current_ticket;
    uint32_t wait_tickets = miliseconds * TICKS_PER_MS;
while((get_current_tick() - start_tick) < wait_tickets) {/* work in progress...*/}}
void sleep_interrupt(uint32_t milliseconds)
{uint32_t end_tick = get_current_tick() + (milliseconds * TICKS_PER_MS);
while (get_current_tick() < end_tick){asm volatile("sti\nhlt");}}








