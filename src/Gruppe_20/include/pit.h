#ifndef PIT_H
#define PIT_H

#include "libc/stdint.h"
#include "interrupts.h"

// PIT Constants
#define PIT_CMD_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40
#define PIT_CHANNEL2_PORT 0x42
#define PC_SPEAKER_PORT 0x61
#define PIT_BASE_FREQUENCY 1193180
#define TARGET_FREQUENCY 1000
#define TICKS_PER_MS (TARGET_FREQUENCY / 1000)

#ifdef __cplusplus
extern "C" {
#endif

// Function declarations
void init_pit();
void sleep_interrupt(uint32_t milliseconds);
void sleep_busy(uint32_t milliseconds);
void play_sound(uint32_t freq);
void stop_sound();


// Hardware I/O
//uint8_t inb(uint16_t port);
//void outb(uint16_t port, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif
