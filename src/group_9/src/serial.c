#include "port_io.h"

// Serial portu başlat
void serial_init() {
    outb(0x3F8 + 1, 0x00);    // Disable all interrupts
    outb(0x3F8 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(0x3F8 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(0x3F8 + 1, 0x00);    //                  (hi byte)
    outb(0x3F8 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(0x3F8 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(0x3F8 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

// Karakter gönder
void debug_serial(char c) {
    while((inb(0x3F8 + 5) & 0x20) == 0); // Boşalmasını bekle
    outb(0x3F8, c);
}

// String gönder
void debug_serial_str(const char* str) {
    while(*str) {
        debug_serial(*str++);
    }
}