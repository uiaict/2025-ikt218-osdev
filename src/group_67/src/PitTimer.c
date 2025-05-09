#include "libc/stdint.h"
#include "libc/util.h"
#include "libc/idt.h"
#include "libc/PitTimer.h"

uint64_t ticks;

void onIrq0(struct InterruptRegisters *regs) {
    ticks += 1;
}

void initTimer() {
    ticks = 0;
    irq_install_handler(0, &onIrq0);

    //119318.16666 Hz

    outPortB(PIT_CMD_PORT, 0xB6);
    outPortB(PIT_CHANNEL0_PORT, (uint8_t) (DIVIDER & 0xFF));
    outPortB(PIT_CHANNEL0_PORT, (uint8_t) ((DIVIDER >> 8) & 0xFF));
}

void sleep_interrupt(uint32_t milliseconds) {
    uint64_t currentTicks = ticks;
    uint64_t ticksToWait = milliseconds * TICKS_PER_MS;
    uint64_t endTicks = currentTicks + ticksToWait;
    while( currentTicks < endTicks) {
        asm volatile("sti;hlt");
        currentTicks = ticks;
    }
}

void sleep_busy(uint32_t milliseconds) {
    uint64_t startTick = ticks;
    uint64_t ticksToWait = milliseconds * TICKS_PER_MS;
    uint64_t elapsedTicks = 0;
    while( elapsedTicks < ticksToWait) {
        while( ticks == startTick + elapsedTicks) {
        }
        elapsedTicks++;
    }
}