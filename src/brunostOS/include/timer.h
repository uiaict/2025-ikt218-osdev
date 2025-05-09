#ifndef TIMER_H
#define TIMER_H

#include "libc/stdint.h"
#include "isr.h"

#define PIT_DATACHANNEL_0 0x40  // Connected to IRQ0
#define PIT_DATACHANNEL_1 0x41  // Used to control refresh rates for DRAM
#define PIT_DATACHANNEL_2 0x42  // Control speaker
#define PIT_COMMAND       0x43  // Write-only, 
#define PIT_REFRESHRATE 1193180

uint32_t get_global_tick();
void init_pit(uint32_t); // Lowest frequency = 18.2Hz
void pit_handler(struct registers);
void busy_sleep(uint32_t);
void interrupt_sleep(uint32_t);

#endif // TIMER_H