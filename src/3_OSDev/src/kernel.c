#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

#include <multiboot2.h>

#include "vga.h"
#include "descriptor_table.h"
#include "interrupts.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Sources: http://www.brokenthorn.com/Resources/OSDev10.html
//          https://wiki.osdev.org/Printing_To_Screen
//          https://www.youtube.com/watch?v=dQqLT4qGiqI&list=PL2EF13wm-hWAglI8rRbdsCPq_wRpYvQQy&index=3

void display_ascii_logo(void) {
    print(0x0B, "   ____   _____ _____             ____  \n");
    print(0x0B, "  / __ \\ / ____|  __ \\           |___ \\ \n");
    print(0x0B, " | |  | | (___ | |  | | _____   ____) | \n");
    print(0x0B, " | |  | |\\___ \\| |  | |/ _ \\ \\ / /__ < \n");
    print(0x0B, " | |__| |____) | |__| |  __/\\ V /___) | \n");
    print(0x0B, "  \\____/|_____/|_____/ \\___| \\_/|____/ \n");
    print(0x0B, "                                       \n");
    print(0x0F, "      Operating System Development     \n");
    print(0x07, "     UiA IKT218 Course Project Team 3  \n");
    print(0x07, "=======================================\n");
    print(0x0F, "\n");
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    // Display Introduction
    Reset();
    
    // Debug message before logo
    display_ascii_logo();
    
    // Initialize GDT, IDT, and IRQ handlers
    init_gdt();
    init_idt();
    init_irq();
    init_irq_handlers();
    enable_interrupts();
    
    // Print "Hello World!" to screen
    print(0x0F, "Hello World!\n");

    while (1) {}
    return 0;

}