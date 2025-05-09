#ifndef PROGRAMMABLE_INTERVAL_TIMER_H
#define PROGRAMMABLE_INTERVAL_TIMER_H

#include "libc/stdint.h"

// PIT konstanter
#define PIT_BASE_FREQUENCY   1193180  // 1.193182 MHz (3579545/3)
#define TARGET_FREQUENCY     1000     // Økt fra 100 Hz til 1000 Hz for bedre lydytelse

// PIT porter
#define PIT_CHANNEL0_PORT    0x40     // Kanal 0 dataport
#define PIT_CHANNEL1_PORT    0x41     // Kanal 1 dataport (ikke i bruk)
#define PIT_CHANNEL2_PORT    0x42     // Kanal 2 dataport (PC høyttaler)
#define PIT_COMMAND_PORT     0x43     // Modus/Kommando register

// PIT kommando byte bits
#define PIT_CHANNEL0         0x00     // Velg kanal 0
#define PIT_LOHI             0x30     // Tilgangsmodus: lav/høy byte
#define PIT_MODE2            0x04     // Modus 2: frekvensgenerator
#define PIT_MODE3            0x06     // Modus 3: firkantbølgegenerator

/**
 * PIT funksjoner
 * 
 * Håndterer systemtid og venting
 */
void init_programmable_interval_timer(void);  // Initialiserer PIT
void timer_handler(void);                     // Håndterer timer interrupts
uint32_t get_current_tick(void);             // Henter nåværende tick-teller
void sleep_busy(uint32_t milliseconds);      // Vent med busy-waiting
void sleep_interrupt(uint32_t milliseconds); // Vent med interrupts

#endif // PROGRAMMABLE_INTERVAL_TIMER_H 