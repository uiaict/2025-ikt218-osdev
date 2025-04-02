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


void display_ascii_logo(void) {
    print("   ____   _____ _____             ____  \n", 0x0B);
    print("  / __ \\ / ____|  __ \\           |___ \\ \n", 0x0B);
    print(" | |  | | (___ | |  | | _____   ____) | \n", 0x0B);
    print(" | |  | |\\___ \\| |  | |/ _ \\ \\ / /__ < \n", 0x0B);
    print(" | |__| |____) | |__| |  __/\\ V /___) | \n", 0x0B);
    print("  \\____/|_____/|_____/ \\___| \\_/|____/ \n", 0x0B);
    print("                                       \n", 0x0B);
    print("      Operating System Development     \n", 0x0F);
    print("     UiA IKT218 Course Project Team 3  \n", 0x07);
    print("=======================================\n", 0x07);
    print("\n", 0x0F);
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    // Display Introduction
    Reset();
    
    // Debug message before logo
    display_ascii_logo();
    
    // Initialize GDT, IDT, and IRQ handlers
    init_gdt();
    init_idt();
    
    // Print "Hello World!" to screen
    print("Hello World!\n", 0x0F);

    while (1) {}
    return 0;

}