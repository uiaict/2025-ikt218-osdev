#pragma once
#include <stdint.h>

// Initierer PIT med 1000 Hz (1 ms per tick)
void init_pit();

// Sover i gitt antall millisekunder ved å bruke busy waiting (høy CPU)
void sleep_busy(uint32_t milliseconds);

// Sover i gitt antall millisekunder ved å bruke interrupts og hlt (lav CPU)
void sleep_interrupt(uint32_t milliseconds);

// Returnerer antall ticks siden oppstart
uint32_t get_tick();
