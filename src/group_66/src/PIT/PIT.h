#ifndef PIT_H
#define PIT_H

// PIT (Programmable Interval Timer) related macros
#define PIT_CMD_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40
#define PIT_CHANNEL1_PORT 0x41
#define PIT_CHANNEL2_PORT 0x42
#define PC_SPEAKER_PORT 0x61
#define PIT_DEFAULT_DIVISOR 0x4E20 // 20000, which gives about 18.2 Hz (1193180 / 20000)

// IRQ0 related macros
#define PIC1_CMD_PORT 0x20
#define PIC1_DATA_PORT 0x20
#define PIC_EOI     0x20        /* End-of-interrupt command code */
 
// Custom sleep function constants
#define PIT_BASE_FREQUENCY 1193180 // 1.193180 Mhz = 1193180 hz
#define TARGET_FREQUENCY 1000 // 1000 hz
#define DIVIDER (PIT_BASE_FREQUENCY / TARGET_FREQUENCY)
#define TICKS_PER_MS (TARGET_FREQUENCY / TARGET_FREQUENCY)

void initPit();

void setTimer(float newCounter);
void sleepInterrupt(uint32_t milliseconds);
void sleepBusy(uint32_t milliseconds);
uint32_t getSystemTicks();

#endif
