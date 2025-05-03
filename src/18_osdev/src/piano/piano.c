#include "piano.h"
#include "../song/SongPlayer.h"  // for play_sound(), stop_sound()
#include "../song/frequencies.h"
#include "libc/monitor.h"
#include "libc/stdbool.h"



void handle_piano_key(unsigned char scancode) {
    switch (scancode) {
        case 0x1E: stop_sound(); play_sound(C4); break; // A
        case 0x1F: stop_sound(); play_sound(D4); break; // S
        case 0x20: stop_sound(); play_sound(E4); break; // D
        case 0x21: stop_sound(); play_sound(F4); break; // F
        case 0x23: stop_sound(); play_sound(G4); break; // H
        case 0x24: stop_sound(); play_sound(A4); break; // J
        case 0x25: stop_sound(); play_sound(B4); break; // K
        case 0x26: stop_sound(); play_sound(C5); break; // L
        default: break;
    }
}


void init_piano() {
    // Optional: print instructions to screen
    monitor_write("Piano mode: Use keys A (C1) S (D1) D(E1) F(F1) H(G1) J(A1) K(B1) L(C2\n");
    monitor_write("Press ESC to exit piano mode\n");
    piano_mode_enabled = true;
}
