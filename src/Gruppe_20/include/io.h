#ifndef IO_H
#define IO_H

#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "libc/common.h"

// Input/Output port functions
static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait(void) {
    // Port 0x80 is used for 'checkpoints' during POST
    outb    (0x80, 0);
}

// Memory-mapped I/O functions
static inline void mmio_write32(uint32_t* addr, uint32_t value) {
    *addr = value;
}

static inline uint32_t mmio_read32(uint32_t* addr) {
    return *addr;
}

// Interrupt control functions
static inline void cli(void) {
    asm volatile ("cli");
}

static inline void sti(void) {
    asm volatile ("sti");
}

// Serial port I/O (for debugging)
void serial_init(void);
void serial_putc(char c);
char serial_getc(void);

// Keyboard status functions
bool keyboard_is_caps_on(void);
bool keyboard_is_shift_pressed(void);

// Screen I/O functions
void screen_clear(void);
void screen_putc(char c);
void screen_puts(const char* str);

#endif // IO_H