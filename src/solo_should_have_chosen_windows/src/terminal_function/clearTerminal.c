#include "terminal/clear.h"
#include "libc/stdint.h"

#define VGA_ADDRESS 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define WHITE_ON_BLACK 0x07

// Function to write to an I/O port (inline assembly)
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Function to move the cursor to a new position
static void move_cursor(uint16_t position) {
    outb(0x3D4, 0x0F); // Select cursor low byte
    outb(0x3D5, (uint8_t)(position & 0xFF)); // Write low byte
    outb(0x3D4, 0x0E); // Select cursor high byte
    outb(0x3D5, (uint8_t)((position >> 8) & 0xFF)); // Write high byte
}

// Function to clear the terminal (clear screen and reset cursor)
void clearTerminal(void) {
    uint16_t *video_memory = (uint16_t*) VGA_ADDRESS;

    // Fill the entire screen with blank spaces
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video_memory[i] = (WHITE_ON_BLACK << 8) | ' ';
    }

    // Reset cursor to (0,0)
    move_cursor(0);
}