#include "piano.h"
#include "display.h"
#include "pcSpeaker.h"
#include "programmableIntervalTimer.h"
#include "interruptHandler.h"
#include "libc/stdbool.h"

/**
 * Viser pianomenyen med instruksjoner
 * 
 * Viser en meny som forklarer hvilke taster som tilsvarer hvilke noter
 * og gir brukeren instruksjoner for bruk.
 */
void show_piano_menu(void) {
    // Tøm skjermen før menyen vises
    display_clear();
    
    // Viser menyheader
    display_write_color("\n", COLOR_WHITE);
    display_write_color("                  PIANO KEYBOARD\n", COLOR_CYAN);
    display_write_color("                  =============\n\n", COLOR_CYAN);
    
    display_write_color("Use the following keys to play notes:\n\n", COLOR_WHITE);
    
    // Viser hovedpianotaster (midtre rad)
    display_write_color("  Main piano keys (middle row):\n", COLOR_YELLOW);
    display_write_color("  ", COLOR_WHITE);
    display_write_color("A-S-D-F-G-H-J-K", COLOR_LIGHT_CYAN);
    display_write_color(" - C4 to C5 (white keys)\n", COLOR_WHITE);
    
    // Viser svarte taster (øverste rad)
    display_write_color("\n  Black keys (top row):\n", COLOR_YELLOW);
    display_write_color("  ", COLOR_WHITE);
    display_write_color("W-E---T-Y-U", COLOR_LIGHT_MAGENTA);
    display_write_color("   - C#4, D#4, F#4, G#4, A#4\n", COLOR_WHITE);
    
    // Viser lavere oktav (nederste rad)
    display_write_color("\n  Lower octave (bottom row):\n", COLOR_YELLOW);
    display_write_color("  ", COLOR_WHITE);
    display_write_color("Z-X-C-V-B-N-M", COLOR_LIGHT_BLUE);
    display_write_color("    - C3 to B3\n\n", COLOR_WHITE);
    
    // Viser avslutningsinstruksjon
    display_write_color("Press ", COLOR_WHITE);
    display_write_color("ESC", COLOR_LIGHT_RED);
    display_write_color(" to return to main menu\n", COLOR_WHITE);
}

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
void handle_piano_keyboard(void) {
    show_piano_menu();

    // Flagg for å spore om en tast er trykket
    bool key_pressed = false;
    char current_key = 0;
    
    // Aktiver interrupts for tastaturinndata
    __asm__ volatile("sti");
    
    // Tøm eventuelle ventende tastaturinndata
    while (keyboard_data_available()) {
        keyboard_getchar();
    }

    // Deaktiver høyttaler initialt for å sikre at den er av
    disable_speaker();
    
    bool running = true;
    uint32_t last_check_time = get_current_tick();
    
    // For å spore tastslipstidspunkt
    uint32_t key_press_time = 0;
    const uint32_t KEY_HOLD_TIMEOUT = 500; // Vurder tast sluppet etter 500ms
    
    while (running) {
        // Hent gjeldende tid for periodiske sjekker
        uint32_t current_time = get_current_tick();
        
        // Behandle tastaturinndata
        if (keyboard_data_available()) {
            char key = keyboard_getchar();
            
            if (key == 27) { // ESC tast
                running = false;
                break;
            }
            
            // Kartlegg taster til frekvenser
            uint32_t frequency = 0;
            
            // Sjekk om dette er en ny tast (ikke allerede trykket)
            if (key == current_key) {
                // Dette er samme tast som holdes nede, oppdater trykktid
                key_press_time = current_time;
                continue;
            }
            
            // Hovedpianotaster på midtre rad
            switch (key) {
                case 'a': case 'A': frequency = C4; break;
                case 's': case 'S': frequency = D4; break;
                case 'd': case 'D': frequency = E4; break;
                case 'f': case 'F': frequency = F4; break;
                case 'g': case 'G': frequency = G4; break;
                case 'h': case 'H': frequency = A4; break;
                case 'j': case 'J': frequency = B4; break;
                case 'k': case 'K': frequency = C5; break;
                
                // Svarte taster
                case 'w': case 'W': frequency = Cs4; break;
                case 'e': case 'E': frequency = Ds4; break;
                case 't': case 'T': frequency = Fs4; break;
                case 'y': case 'Y': frequency = Gs4; break;
                case 'u': case 'U': frequency = As4; break;
                
                // Lavere oktav
                case 'z': case 'Z': frequency = C3; break;
                case 'x': case 'X': frequency = D3; break;
                case 'c': case 'C': frequency = E3; break;
                case 'v': case 'V': frequency = F3; break;
                case 'b': case 'B': frequency = G3; break;
                case 'n': case 'N': frequency = A3; break;
                case 'm': case 'M': frequency = B3; break;
            }
            
            if (frequency > 0) {
                // Ny tast trykket, stopp forrige note
                if (key_pressed) {
                    disable_speaker();
                }
                
                // Spill ny note
                enable_speaker();
                play_sound(frequency);
                key_pressed = true;
                current_key = key;
                key_press_time = current_time;
            }
        }
        
        // Sjekk om tast skal slippes
        if (key_pressed && (current_time - key_press_time) >= KEY_HOLD_TIMEOUT) {
            disable_speaker();
            key_pressed = false;
            current_key = 0;
        }
        
        // Vent litt for å unngå for høy CPU-bruk
        sleep_interrupt(5); // Redusert fra 10ms til 5ms for mer responsiv kontroll
    }
    
    // Sikre at høyttaler er deaktivert når vi avslutter
    disable_speaker();
} 