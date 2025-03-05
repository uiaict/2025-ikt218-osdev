#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

// GDT initialization (defined elsewhere):
#include "arch/i386/GDT/gdt.h"

// VGA driver / animation:
#include "drivers/VGA/vga.h" 

// If your bootloader provides a multiboot structure:
struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Entry point
int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize GDT if you have one
    initGdt();

    // Clear screen to default color (light grey)
    Reset();

    // Show our two-frame ASCII animation
    show_animation();

    // Print a final message
    print("hello world\r\n");

    // Never return from main in a bare-metal OS
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
