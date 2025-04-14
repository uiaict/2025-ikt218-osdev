#include "game/input_buffer.h"
#include "libc/scrn.h"
#include "libc/io.h"
#include "libc/isr_handlers.h"
#include "pit/pit.h"

extern char scancode_to_ascii[128];
extern char scancode_to_ascii_shift[128];

void get_input(char* buffer, int max_len) {
    int index = 0;
    bool done = false;

    while (!done && index < max_len - 1) {
        uint8_t scancode = 0;

        // Vent på ny tastetrykk
        do {
            scancode = inb(0x60);
        } while (scancode == 0 || (scancode & 0x80)); // ignorer break codes

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

                // Flytt markør tilbake, skriv space for å slette, flytt tilbake igjen
                terminal_write("\b \b", VGA_COLOR(15, 0));
            }
        }
        // === VANLIG TEGN ===
        else {
            char c = scancode_to_ascii[scancode];
            if (c) {
                buffer[index++] = c;
                char str[2] = {c, '\0'};
                terminal_write(str, VGA_COLOR(15, 0));
            }
        }

        // Vent til tast slippes
        while (!(inb(0x60) & 0x80)) {
            __asm__ volatile("hlt");
        }

        // Debounce pause
        sleep_interrupt(30);
    }

    buffer[index] = '\0';
}
