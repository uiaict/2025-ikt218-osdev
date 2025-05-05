// === port_io.c ===
#include "port_io.h"
#include "terminal.h"

#define PIT_CHANNEL2_PORT 0x42
#define PIT_COMMAND_PORT  0x43
#define PC_SPEAKER_PORT   0x61

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
void enable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 3); // Set bits 0 and 1
}

void disable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC); // Clear bits 0 and 1
}

void play_sound(uint32_t frequency) {
    terminal_write("play_sound: ");
    terminal_putint(frequency);
    terminal_write(" Hz\n");

    if (frequency == 0) return;

    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0xB6); // Set PIT channel 2, mode 3 (square wave)
    outb(0x42, (uint8_t)(divisor & 0xFF));        // Low byte
    outb(0x42, (uint8_t)((divisor >> 8) & 0xFF)); // High byte

    enable_speaker();
}

void stop_sound() {
    disable_speaker(); // Disconnect PIT from speaker
}