#include "interrupts/pit.h"

#include "libc/io.h"
#include "libc/stdint.h"

void pit_init() {
    uint16_t divisor = PIT_BASE_FREQUENCY / TARGET_FREQUENCY;

    outb(PIT_CMD_PORT, 0x34);
    
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF); // Low byte
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF); // High byte
}