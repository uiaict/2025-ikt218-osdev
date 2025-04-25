#include "pit.h"
#include "../idt/idt.h"
#include <libc/stdint.h>
#include "../utils/utils.h"

static volatile uint32_t ticks = 0;  // Marked volatile because it's updated in interrupt context

// IRQ0 handler without context (matches your irq_install_handler signature)
void pit_irq_handler(struct InterruptRegisters* regs) {
    (void)regs; // Prevent unused parameter warning
    ticks++;
}

void init_pit() {
    // Use your installed handler registration function
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
    mafiaPrint("=== PIT TEST START ===\n");

    init_pit();
    mafiaPrint("PIT initialized.\n");

    // ---- Test 1: Wait 1 second using sleep_interrupt ----
    mafiaPrint("[Test 1] sleep_interrupt(1000)...\n");
    uint32_t start = get_ticks();
    sleep_interrupt(1000);
    uint32_t end = get_ticks();
    mafiaPrint("Elapsed (interrupt): %d ticks (expected: ~1000)\n", end - start);

    // ---- Test 2: Wait 1 second using sleep_busy ----
    mafiaPrint("[Test 2] sleep_busy(1000)...\n");
    start = get_ticks();
    sleep_busy(1000);
    end = get_ticks();
    mafiaPrint("Elapsed (busy): %d ticks (expected: ~1000)\n", end - start);

    // ---- Test 3: Live uptime counter (3 seconds) ----
    mafiaPrint("[Test 3] Live uptime (3 seconds):\n");
    uint32_t last = get_ticks() / 1000;
    while ((get_ticks() - start) < 3000) {
        uint32_t current = get_ticks() / 1000;
        if (current != last) {
            mafiaPrint("Uptime: %d seconds\n", current);
            last = current;
        }
    }

    // ---- Test 4: Precision test ----
    mafiaPrint("[Test 4] Precision sleep test:\n");
    const uint32_t durations[] = {1, 10, 100, 250, 500};
    for (int i = 0; i < sizeof(durations)/sizeof(durations[0]); ++i) {
        mafiaPrint("  sleep_interrupt(%d)... ", durations[i]);
        start = get_ticks();
        sleep_interrupt(durations[i]);
        end = get_ticks();
        mafiaPrint("%d ticks elapsed\n", end - start);
    }

    mafiaPrint("=== PIT TEST END ===\n");
}