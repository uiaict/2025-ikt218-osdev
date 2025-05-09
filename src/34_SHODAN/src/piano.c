#include "port_io.h"
#include "terminal.h"
#include "song/frequencies.h"
#include "pit.h"

uint32_t scancode_to_frequency(uint8_t scancode) {
    switch (scancode) {
        case 0x1E: return C4; // A
        case 0x1F: return D4; // S
        case 0x20: return E4; // D
        case 0x21: return F4; // F
        case 0x22: return G4; // G
        case 0x23: return A4; // H
        case 0x24: return B4; // J
        case 0x25: return C5; // K
        case 0x26: return D5; // L
        case 0x27: return E5; // ;
        default: return 0;
    }
}


void run_piano() {
    terminal_write("=== Simple PC Speaker Piano ===\n");
    terminal_write("Use keys A S D F G H J K L ;\n");

    while (1) {
        if (inb(0x64) & 0x1) {
            uint8_t scancode = inb(0x60);
            uint32_t freq = scancode_to_frequency(scancode);

            if (freq != 0) {
                terminal_write("Note: ");
                terminal_putint(freq);
                terminal_write(" Hz\n");

                play_sound(freq);
                sleep_busy(300); // brief tone
                stop_sound();
            }
        }
    }
}
