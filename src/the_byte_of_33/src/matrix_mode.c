#include "matrix_mode.h"
#include "io.h"            // for clear_screen()
#include <libc/stdint.h>
#include <libc/stdbool.h>

#define COLS       80
#define ROWS       25
#define ATTR_HEAD  0x0F    // bright white on black
#define MAX_TAIL   8       // maximum tail length

// Tail colors: green, blue, red, yellow
static const uint8_t tail_colors[] = { 0x0A, 0x09, 0x0C, 0x0E };
static int color_index = 0;

// simple LFSR for randomness
static uint32_t lfsr = 0x12345678;
static uint32_t rnd(void) {
    lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xB400u);
    return lfsr;
}

void matrix_mode(void) {
    __asm__ volatile("cli");     // disable interrupts
    clear_screen();
    volatile uint16_t *vga = (volatile uint16_t*)0xB8000;

    // Per-column state
    int head_row[COLS], tail_len[COLS], speed_div[COLS];
    for (int c = 0; c < COLS; c++) {
        head_row[c] = rnd() % ROWS;
        tail_len[c] = 3 + (rnd() % (MAX_TAIL - 2));
        speed_div[c] = 1 + (rnd() % 3);
    }

    // Age buffer
    uint8_t agebuf[ROWS][COLS] = {{0}};
    bool rave_mode = false;
    uint32_t frame = 0;

    while (1) {
        // In rave mode, cycle color every frame
        if (rave_mode) {
            color_index = frame % (sizeof(tail_colors)/sizeof(tail_colors[0]));
        }
        uint8_t ATTR_TAIL = tail_colors[color_index];

        // Fade & redraw all but bottom row
        for (int r = 0; r < ROWS-1; r++) {
            for (int c = 0; c < COLS; c++) {
                if (agebuf[r][c]) {
                    agebuf[r][c]--;
                    char ch = (rnd() & 1) ? '1' : '0';
                    uint8_t attr = (agebuf[r][c] == tail_len[c])
                                   ? ATTR_HEAD
                                   : ATTR_TAIL;
                    vga[r*COLS + c] = (attr << 8) | (uint8_t)ch;
                } else {
                    vga[r*COLS + c] = (ATTR_TAIL << 8) | ' ';
                }
            }
        }

        // Advance each column’s head
        for (int c = 0; c < COLS; c++) {
            if ((frame % speed_div[c]) == 0) {
                int r = head_row[c];
                agebuf[r][c] = tail_len[c];
                head_row[c] = (r + 1) % ROWS;
                if (head_row[c] == 0) {
                    tail_len[c] = 3 + (rnd() % (MAX_TAIL - 2));
                    speed_div[c] = 1 + (rnd() % 3);
                }
            }
        }

        // Bottom prompt (row ROWS-1)
        {
            const char *prompt =
                "Press Q to quit | Press C to change color | Press R to toggle Rave Mode |";
            int len = 0;
            while (prompt[len]) len++;
            int start = (COLS - len) / 2;
            for (int i = 0; i < len && start + i < COLS; i++) {
                vga[(ROWS-1)*COLS + start + i] =
                    (ATTR_HEAD << 8) | (uint8_t)prompt[i];
            }
        }

        // Non-blocking PS/2 scan: one scancode per frame
        if (inb(0x64) & 1) {
            uint8_t sc = inb(0x60);
            // only handle make codes
            if (!(sc & 0x80)) {
                switch (sc) {
                    case 0x10:  // Q
                        clear_screen();
                        __asm__ volatile("sti");  // re-enable interrupts
                        return;
                    case 0x2E:  // C
                        if (!rave_mode) {
                            color_index = (color_index + 1)
                                % (sizeof(tail_colors)/sizeof(tail_colors[0]));
                        }
                        break;
                    case 0x13:  // R
                        rave_mode = !rave_mode;
                        break;
                    default:
                        break;
                }
            }
        }

        // Delay tick
        for (volatile uint32_t d = 0; d < 2000000; d++)
            __asm__ volatile("nop");

        frame++;
    }
}
