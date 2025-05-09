#include "interrupts/pit.h"
#include "interrupts/isr.h"
#include "interrupts/io.h"
#include "libc/stdio.h"
#include "music/song.h"

extern bool is_song_playing;
volatile uint32_t tick = 0; // PIT tick counter

// PIT interrupt handler
void pitHandler(registers_t regs) {
    tick++;

    if (is_song_playing) {
        update_song_tick(); // Play next note if song is active
    }
}

// Returns the current tick count
uint32_t getCurrentTick() {
    return tick;
}

// Initializes PIT to a set frequency and registers handler
void initPit() {
    outb(PIT_CMD_PORT, 0x36);
    outb(PIT_CHANNEL0_PORT, (uint8_t)(DIVIDER & 0xFF));
    outb(PIT_CHANNEL0_PORT, (uint8_t)((DIVIDER >> 8) & 0xFF));

    registerInterruptHandler(IRQ0, &pitHandler);
}

// Sleeps using busy-wait loop
void sleepBusy(uint32_t ms) {
    uint32_t startTick = getCurrentTick();
    uint32_t ticksToWait = ms * TICKS_PER_MS;
    uint32_t endTick = startTick + ticksToWait;

    if (endTick < startTick) {
        while (getCurrentTick() >= startTick) {}
        while (getCurrentTick() < endTick) {}
    } else {
        while (getCurrentTick() < endTick) {}
    }
}

// Sleeps using hlt + sti (interrupt-based)
void sleepInterrupt(uint32_t ms) {
    uint32_t startTick = getCurrentTick();
    uint32_t ticksToWait = ms * TICKS_PER_MS;
    uint32_t endTick = startTick + ticksToWait;

    if (endTick < startTick) {
        while (getCurrentTick() >= startTick) {
            asm volatile ("sti\n\thlt\n\t");
        }
        while (getCurrentTick() < endTick) {
            asm volatile ("sti\n\thlt\n\t");
        }
    } else {
        while (getCurrentTick() < endTick) {
            asm volatile ("sti\n\thlt\n\t");
        }
    }
}
