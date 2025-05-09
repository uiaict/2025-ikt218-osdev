#include "pit.h"

static volatile uint32_t ticks = 0;

// IRQ0 handler
void pit_irq_handler(struct InterruptRegisters *regs) {
    (void)regs; // Prevent unused parameter warning
    ticks++;
}

void init_pit() {
    // Use installed handler function
    irq_install_handler(IRQ0, pit_irq_handler);

    // Send PIT control word: channel 0, access mode lobyte/hibyte, mode 3 (square wave)
    outPortB(PIT_CMD_PORT, 0x36);

    // Calculate and send divisor bytes
    uint16_t divisor = PIT_BASE_FREQUENCY / TARGET_FREQUENCY;
    uint8_t l_divisor = divisor & 0xFF;
    uint8_t h_divisor = (divisor >> 8) & 0xFF;

    outPortB(PIT_CHANNEL0_PORT, l_divisor);
    outPortB(PIT_CHANNEL0_PORT, h_divisor);
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t current_tick = ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_ticks = current_tick + ticks_to_wait;

    while (ticks < end_ticks) {
        asm volatile("sti");
        asm volatile("hlt");
    }
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;

    while (elapsed_ticks < ticks_to_wait) {
        while (ticks == start_tick + elapsed_ticks) {
            // Busy wait
        }
        elapsed_ticks++;
    }
}

uint32_t get_ticks() {
    return ticks;
}

void test_pit() {
    Print("PIT test started\n");

    init_pit();
    Print("PIT initialized\n");

    // Test 1: Wait 1 second using sleep_interrupt
    Print("[Test 1] sleep_interrupt(1000)...\n");
    uint32_t start = get_ticks();
    sleep_interrupt(1000);
    uint32_t end = get_ticks();
    Print("Elapsed (interrupt): %d ticks (expected: ~1000)\n", end - start);

    // Test 2: Wait 1 second using sleep_busy
    Print("[Test 2] sleep_busy(1000)...\n");
    start = get_ticks();
    sleep_busy(1000);
    end = get_ticks();
    Print("Elapsed (busy): %d ticks (expected: ~1000)\n", end - start);

    // Test 3: Live uptime counter (3 seconds)
    Print("[Test 3] Live uptime (3 seconds):\n");
    uint32_t last = get_ticks() / 1000;
    while ((get_ticks() - start) < 3000) {
        uint32_t current = get_ticks() / 1000;
        if (current != last) {
            Print("Uptime: %d seconds\n", current);
            last = current;
        }
    }

    // Test 4: Precision test
    Print("[Test 4] Precision sleep test:\n");
    const uint32_t durations[] = {1, 10, 100, 250, 500};
    for (size_t i = 0; i < sizeof(durations) / sizeof(durations[0]); ++i) {
        Print("  sleep_interrupt(%d)... ", durations[i]);
        start = get_ticks();
        sleep_interrupt(durations[i]);
        end = get_ticks();
        Print("%d ticks elapsed\n", end - start);
    }
    Print("PIT test ended\n");
}