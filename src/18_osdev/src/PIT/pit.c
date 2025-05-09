#include "pit.h"

static volatile uint64_t tick_count = 0;

void pit_callback(registers_t regs) {
    tick_count++;
}

void init_pit() {
    register_interrupt_handler(IRQ0, &pit_callback);
    uint16_t divisor = DIVIDER;

    // Send command byte
    outb(PIT_CMD_PORT, 0x36); // Channel 0, Access mode: lobyte/hibyte, Mode 3 (square wave)

    // Send frequency divisor (low byte first, then high byte)
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));        // Low byte
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF)); // High byte

    tick_count = 0;
}

uint64_t get_tick() {
    return tick_count;
}

void sleep_interrupt(uint32_t milliseconds) {
    uint64_t end_tick = get_tick() + (milliseconds * TICKS_PER_MS);

    while (get_tick() < end_tick) {
        asm volatile("sti\n\thlt");
    }
}

void sleep_busy(uint32_t milliseconds) {
    uint64_t start_tick = get_tick();
    uint64_t ticks_to_wait = milliseconds * TICKS_PER_MS;

    while ((get_tick() - start_tick) < ticks_to_wait) {
        // do nothing (busy wait)
    }
}

void test_pit_timing() {
    monitor_write("Starting PIT timing tests...\n");
    
    // Test sleep_interrupt function
    monitor_write("Testing sleep_interrupt(5000) - Should take 5 seconds\n");
    uint32_t start_tick = tick_count;
    sleep_interrupt(5000);
    uint32_t end_tick = tick_count;
    uint32_t elapsed_ms = end_tick - start_tick;
    
    monitor_write("sleep_interrupt(5000) took ");
    monitor_write_dec(end_tick - start_tick);
    monitor_write(" ticks (");
    monitor_write_dec(elapsed_ms);
    monitor_write(" ms)\n");
    
    // Let system stabilize a bit
    sleep_interrupt(1000);
    
    // Test sleep_busy function
    monitor_write("Testing sleep_busy(2000) - Should take 2 seconds\n");
    start_tick = tick_count;
    sleep_busy(2000);
    end_tick = tick_count;
    elapsed_ms = end_tick - start_tick;
    
    monitor_write("sleep_busy(2000) took ");
    monitor_write_dec(end_tick - start_tick);
    monitor_write(" ticks (");
    monitor_write_dec(elapsed_ms);
    monitor_write(" ms)\n");
}