// === port_io.c ===
// Provides low-level port I/O operations and PC speaker control
#include "port_io.h"
#include "terminal.h"

// PIT and PC speaker I/O ports
#define PIT_CHANNEL2_PORT 0x42  // PIT channel 2 data port
#define PIT_COMMAND_PORT  0x43 // PIT mode/command register
#define PC_SPEAKER_PORT   0x61 // PC speaker control port

// Sends a byte to the specified I/O port
void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Reads a byte from the specified I/O port
uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Enables the PC speaker by setting bits 0 and 1 of port 0x61
void enable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 3); // Set bits 0 and 1
}

// Disables the PC speaker by clearing bits 0 and 1 of port 0x61
void disable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC); // Clear bits 0 and 1
}

// Configures PIT channel 2 to generate a tone at the specified frequency
void play_sound(uint32_t frequency) {
    terminal_write("play_sound: ");
    terminal_putint(frequency);
    terminal_write(" Hz\n");

    if (frequency == 0) return;

    // PIT base frequency divided by desired frequency gives divisor
    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0xB6); // Set PIT channel 2, mode 3 (square wave)
    outb(0x42, (uint8_t)(divisor & 0xFF));        // Low byte
    outb(0x42, (uint8_t)((divisor >> 8) & 0xFF)); // High byte

    // Enable the speaker to start the tone
    enable_speaker();
}
// Stops sound output by disabling the speaker
void stop_sound() {
    disable_speaker(); // Disconnect PIT from speaker
}