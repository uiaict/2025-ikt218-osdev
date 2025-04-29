#ifndef PIANO_H
#define PIANO_H

#include "libc/stdint.h"
#include "frequencies.h"

/**
 * Piano funksjonalitet
 * 
 * Implementerer et virtuelt piano som bruker tastaturet som input
 * og PC-høyttaleren for lydgenerering.
 */

/**
 * Viser pianomenyen med instruksjoner
 * 
 * Viser en meny som forklarer hvilke taster som tilsvarer hvilke noter
 */
void show_piano_menu(void);

/**
 * Håndterer pianotastaturinndata og spiller noter
 * 
 * Lytter etter tastetrykk og spiller tilsvarende noter via PC-høyttaleren.
 * Bruker følgende tastaturlayout:
 * - Midtre rad (A-S-D-F-G-H-J-K): Hvite taster (C4 til C5)
 * - Øverste rad (W-E-T-Y-U): Svarte taster (C#4, D#4, F#4, G#4, A#4)
 * - Nederste rad (Z-X-C-V-B-N-M): Lavere oktav (C3 til B3)
 * 
 * Trykk ESC for å avslutte.
 */
void handle_piano_keyboard(void);

#endif /* PIANO_H */ 