#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "GDT.h"
#include "printf.h"
#include <multiboot2.h>



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static void move_cursor() {
    unsigned short pos = cursor_y * VGA_WIDTH + cursor_x;

    // Porter for VGA markør
    asm volatile ("outb %0, %1" : : "a" (0x0F), "Nd" (0x3D4));
    asm volatile ("outb %0, %1" : : "a" (pos & 0xFF), "Nd" (0x3D5));
    asm volatile ("outb %0, %1" : : "a" (0x0E), "Nd" (0x3D4));
    asm volatile ("outb %0, %1" : : "a" ((pos >> 8) & 0xFF), "Nd" (0x3D5));
}

void some_function() {
    outb(0x20, 0x20); // Example usage of outb
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    // Initialiser GDT
    gdt_init();

    clear_screen();
    printf("Hello, Kernel!\n");
    printf("Hello World\n");

    return 0;

}