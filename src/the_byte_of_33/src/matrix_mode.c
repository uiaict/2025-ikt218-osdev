#include "matrix_mode.h"
#include "io.h"
#include "kernel/boot_art.h"
#include <libc/stdint.h>
#include <libc/stdbool.h>

#define COLS       80
#define ROWS       25
#define ATTR_HEAD  0x0F    // bright white on black
#define ATTR_TAIL  0x0A    // green on black
#define MAX_TAIL   8       // maximum tail length

// simple LFSR for randomness
static uint32_t lfsr = 0x12345678;
static uint32_t rnd(void) {
    lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xB400u);
    return lfsr;
}

// nonblocking check for 'Q'
static bool q_pressed(void) {
    if (inb(0x64) & 1) {
        uint8_t sc = inb(0x60);
        if ((sc & 0x7F) == 0x10) return true;
    }
    return false;
}

void matrix_mode(void) {
    __asm__ volatile("cli");
    __clear_screen();
    volatile uint16_t *vga = (volatile uint16_t*)0xB8000;

    // Track for each column: current head row, tail length, speed divisor
    int head_row[COLS];
    int tail_len[COLS];
    int speed_div[COLS];
    for (int c = 0; c < COLS; c++) {
        head_row[c] = rnd() % ROWS;
        tail_len[c] = 3 + (rnd() % (MAX_TAIL - 2)); 
        speed_div[c] = 1 + (rnd() % 3); // 1=fast, 3=slow
    }

    // Age buffer: how “fresh” each cell is (0=blank → MAX_TAIL=head)
    uint8_t agebuf[ROWS][COLS] = {{0}};

    uint32_t frame = 0;
    while (1) {
        // Fade all cells by 1 (clearing when age=0), drawing 0/1
        for (int r = 0; r < ROWS; r++) {
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

        // Advance each head on its own speed
        for (int c = 0; c < COLS; c++) {
            if ((frame % speed_div[c]) == 0) {
                int r = head_row[c];
                agebuf[r][c] = tail_len[c];          // reset head age
                head_row[c] = (r + 1) % ROWS;        // move down
                // when wrapping, randomize tail & speed
                if (head_row[c] == 0) {
                    tail_len[c] = 3 + (rnd() % (MAX_TAIL - 2));
                    speed_div[c] = 1 + (rnd() % 3);
                }
            }
        }

        // Exit on Q
        if (q_pressed()) {
            __clear_screen();
            __asm__ volatile("sti");
            return;
        }

        // Delay tick
        for (volatile uint32_t d = 0; d < 2000000; d++)
            __asm__ volatile("nop");

        frame++;
    }
}
