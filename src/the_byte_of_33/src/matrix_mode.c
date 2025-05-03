#include "matrix_mode.h"
#include "io.h"
#include "kernel/boot_art.h"
#include <libc/stdint.h>
#include <libc/stdbool.h>


// Poll PS/2 status port until a scancode arrives, then return it
static uint8_t wait_scancode(void) {
    while (!(inb(0x64) & 1)) {
        __asm__ volatile("hlt");
    }
    return inb(0x60);
}

// Ignore break codes (>=0x80), return only make codes
static uint8_t wait_make(void) {
    uint8_t sc;
    do {
        sc = wait_scancode();
    } while (sc & 0x80);
    return sc;
}


// Matrix mode: disable IRQ handler, animate, quit on Q
void matrix_mode(void) {
    __asm__ volatile("cli");  // disable all IRQs
    __clear_screen();
    volatile uint16_t* vga = (uint16_t*)0xB8000;
    const uint8_t ATTR = 0x0A;

    int drops[80];
    uint32_t lfsr = 0xBEEF;
    for (int c = 0; c < 80; c++) {
        lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xB400u);
        drops[c] = lfsr % 25;
    }

    while (1) {
        for (int c = 0; c < 80; c++) {
            int r = drops[c] - 1;
            if (r >= 0 && r < 25)
                vga[r*80 + c] = (ATTR<<8) | ' ';
        }
        for (int c = 0; c < 80; c++) {
            lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xB400u);
            char ch = (lfsr & 1u) ? '1' : '0';
            int r = drops[c];
            if (r >= 0 && r < 25)
                vga[r*80 + c] = (ATTR<<8) | (uint8_t)ch;
            drops[c] = (r + 1) % 25;
        }
        // non-blocking Q check
        if (inb(0x64) & 1) {
            uint8_t sc = inb(0x60);
            if ((sc & 0x7F) == 0x10) {  // 'Q'
                __clear_screen();
                __asm__ volatile("sti");  // re-enable IRQs
                return;
            }
        }
        for (volatile uint32_t d = 0; d < 5000000; d++)
            __asm__ volatile("nop");
    }
}