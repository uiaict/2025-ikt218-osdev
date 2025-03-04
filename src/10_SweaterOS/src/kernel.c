#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

#include "descriptorTables.h"
#include "miscFuncs.h"
#include "testFuncs.h"

// Multiboot2 magic number - this needs to match what the bootloader passes
#define MULTIBOOT2_MAGIC 0x36d76289  // This is the correct value from multiboot2.h

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize terminal first to clear the screen
    terminal_initialize();
    
    // Display heading
    terminal_write_color("SweaterOS - Kernel Startup\n", COLOR_YELLOW);
    terminal_write_color("=======================\n\n", COLOR_YELLOW);
    
    // Check if magic is correct
    char magic_str[11];
    hexToString(magic, magic_str);
    
    // Display magic number
    terminal_write_color("Magic: ", COLOR_WHITE);
    if (magic == MULTIBOOT2_MAGIC) {
        terminal_write_color(magic_str, COLOR_GREEN);
        terminal_write_color(" (Correct)", COLOR_GREEN);
    } else {
        terminal_write_color(magic_str, COLOR_RED);
        terminal_write_color(" (Error - expected 0x36d76289)", COLOR_RED);
    }
    terminal_write_char('\n');
    
    // Initialize GDT
    terminal_write_color("Initializing GDT... ", COLOR_WHITE);
    initializer_GDT();
    terminal_write_color("DONE\n", COLOR_GREEN);
    
    // Initialize IDT
    terminal_write_color("Initializing IDT... ", COLOR_WHITE);
    initializer_IDT();
    terminal_write_color("DONE\n", COLOR_GREEN);
    
    // Success message
    terminal_write_char('\n');
    terminal_write_color("System initialization complete.\n", COLOR_LIGHT_CYAN);
    
    // Run all tests
    run_all_tests();
    
    // Infinite loop
    while(1) {}
    
    return 0;
}