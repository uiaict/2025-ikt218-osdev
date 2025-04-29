#pragma once
#include <stdint.h>

// Initierer PIT med 1000 Hz (1 ms per tick)
void init_pit(void);

// Sover i gitt antall millisekunder med busy-waiting (høy CPU-bruk)
void sleep_busy(uint32_t milliseconds);

// Sover i gitt antall millisekunder med interrupts og HLT (lav CPU-bruk)
void sleep_interrupt(uint32_t milliseconds);

// Returnerer antall millisekund-ticks siden oppstart
uint32_t get_tick(void);
