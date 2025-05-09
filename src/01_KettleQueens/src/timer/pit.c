#include "kernel/pit.h"
#include "util.h"
#include "interrupt/idt.h"
#include "interrupt/interrupts.h"
#include "printing/terminal.h"

uint32_t pit_ticks; 

void pit_handler(struct InterruptRegisters *regs) {
    pit_ticks++;
    outb(PIC1_CMD_PORT, PIC_EOI); 
}

void init_pit() {
    pit_ticks = 0;
    irq_install_handler(0, &pit_handler); 

    outb(PIT_CMD_PORT, 0x36); // Square wave
    outb(PIT_CHANNEL0_PORT, (uint8_t)(DIVIDER & 0xFF)); // Low byte
    outb(PIT_CHANNEL0_PORT, (uint8_t)((DIVIDER >> 8) & 0xFF)); // High byte
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t current_ticks = pit_ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_ticks = current_ticks + ticks_to_wait;
    while (current_ticks < end_ticks) {
        asm volatile ("sti"); 
        asm volatile ("hlt"); // Halt CPU until next interrupt
        current_ticks = pit_ticks;
    }
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start_ticks = pit_ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;
    while (elapsed_ticks < ticks_to_wait) {
        while (pit_ticks == start_ticks + elapsed_ticks) {
            // Busy wait   
        }
       elapsed_ticks++;
    }
}

void test_waits(){
    terminal_write("Testing busy sleep 5s \n");
    sleep_busy(5000);
    terminal_write("Awake \n");
    terminal_write("Testing interrupt sleep 5s \n");
    sleep_interrupt(5000);
    terminal_write("Awake \n");
}

