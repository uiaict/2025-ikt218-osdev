#include "game/input_buffer.h"
#include "libc/scrn.h"
#include "libc/io.h"
#include "libc/isr_handlers.h"
#include "pit/pit.h"

extern char scancode_to_ascii[128];
extern char scancode_to_ascii_shift[128];


// Leser input fra tastatur og lagrer det i buffer (maks max_len - 1 tegn + null-terminator)
// Støtter Enter for avslutning, backspace for sletting og vanlig tekstinput
void get_input(char* buffer, int max_len) {
    int index = 0;
    bool done = false;
    bool shift_pressed = false;

    while (!done && index < max_len - 1) {
        uint8_t scancode = 0;

        // Vent på ny tastetrykk
        do {
            scancode = inb(0x60);
        } while (scancode == 0);

        // === SHIFT-NED ===
        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = true;
            continue;
        }
        // === SHIFT-OPP ===
        else if (scancode == 0xAA || scancode == 0xB6) {
            shift_pressed = false;
            continue;
        }

        // === IGNORER BREAK-CODES ===
        if (scancode & 0x80) {
            continue;
        }

        // === ENTER ===
        if (scancode == 0x1C) {
            buffer[index] = '\0';
            printf("\n");
            done = true;
        }
        // === BACKSPACE ===
        else if (scancode == 0x0E) {
            if (index > 0) {
                index--;
                printf("\b \b");
            }
        }
        // === TEGN ===
        else {
            char c = shift_pressed ? scancode_to_ascii_shift[scancode] : scancode_to_ascii[scancode];
            if (c) {
                buffer[index++] = c;
                char str[2] = {c, '\0'};
                printf(str);
            }
        }

        // Vent til tast slippes
        while (!(inb(0x60) & 0x80)) {
            __asm__ volatile("hlt");
        }

        // Debounce
        sleep_interrupt(30);
    }

    buffer[index] = '\0';
}
