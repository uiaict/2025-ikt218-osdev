#include <libc/system.h>

#include "utils.h"       // get_input, outPortB, inPortB
#include "printf.h"      // Print, putc
#include "monitor.h"     // clear_screen, move_cursor, terminal_row/column
#include "pit.h"         // sleep_interrupt
#include "keyboard.h"    // ctrlEnabled, altEnabled

#define EDITOR_BUF_SIZE 4096
#define COM1_PORT       0x3F8

static inline void serial_init(void) {
    outPortB(COM1_PORT+1, 0x00);    // disable all interrupts
    outPortB(COM1_PORT+3, 0x80);    // enable DLAB (set baud rate divisor)
    outPortB(COM1_PORT+0, 0x03);    // divisor = 3 (lo byte) 38400 baud
    outPortB(COM1_PORT+1, 0x00);    // (hi byte)
    outPortB(COM1_PORT+3, 0x03);    // 8 bits, no parity, one stop bit
    outPortB(COM1_PORT+2, 0xC7);    // enable FIFO, clear them, with 14-byte threshold
    outPortB(COM1_PORT+4, 0x0B);    // IRQs enabled, RTS/DSR set
}

static inline void serial_write_char(char c) {
    // vent til Transmitter Holding Register er tom
    while ((inPortB(COM1_PORT+5) & 0x20) == 0);
    outPortB(COM1_PORT+0, c);
}

void save_buffer_to_serial(const char *buf, size_t len) {
    serial_init();
    for (size_t i = 0; i < len; i++)
        serial_write_char(buf[i]);
}


void editor_mode(void) {
    char buf[EDITOR_BUF_SIZE];
    size_t len = 0;
    bool finished = false;
    bool save = false;

    clear_screen();
    Print("Enkel teksteditor, Ctrl for lagre, Alt for avbryte\n\n");
    move_cursor();

    while (!finished && len + 1 < EDITOR_BUF_SIZE) {
        // 1) Sjekk om brukeren vil lagre eller avbryte
        if (ctrlEnabled) {
            save = true;
            finished = true;
            break;
        }
        if (altEnabled) {
            save = false;
            finished = true;
            break;
        }
        // 2) Les vanlige tegn når buffer ikke er tom
        if (!keyboard_buffer_empty()) {
            char c = get_char();
            if (c == '\b') {  // backspace
                if (len > 0) {
                    len--;
                    // flytt cursor én plass tilbake
                    if (terminal_column > 0) {
                        terminal_column--;
                    } else {
                        terminal_row--;
                        terminal_column = SCREEN_WIDTH - 1;
                    }
                    move_cursor();
                    putc(' ');
                    move_cursor();
                }
            } else {
                buf[len++] = c;
                putc(c);
            }
        } else {
            asm volatile("hlt");
        }
    }
    buf[len] = '\0';

    // 3) Lagre eller avbryt
    if (save && len > 0) {
        Print("\n\nLagrer til fil via serial...\n");
        save_buffer_to_serial(buf, len);
        Print("Ferdig. Restart for å se fil på vert.\n");
    } else {
        Print("\n\nAvbrutt eller ingenting å lagre.\n");
    }
    // Når du returnerer her, kommer du tilbake til meny-loopen
}