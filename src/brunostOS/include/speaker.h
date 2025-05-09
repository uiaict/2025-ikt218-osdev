#ifndef SPEAKER_H
#define SPEAKER_H

#include "libc/stdint.h"

#define IO_PORT 0x61
#define PIT_DATACHANNEL_0 0x40  // Connected to IRQ0
#define PIT_DATACHANNEL_1 0x41  // Used to control refresh rates for DRAM
#define PIT_DATACHANNEL_2 0x42  // Control speaker
#define PIT_COMMAND       0x43  // Write-only, 
#define PIT_REFRESHRATE 1193180

void enable_speaker();
void disable_speaker();
void play_sound(uint32_t);
void stop_sound();
void beep();

#endif // SPEAKER_H