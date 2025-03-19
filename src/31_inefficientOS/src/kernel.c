#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "gdt.h"
#include "terminal.h"
#include "common.h"
#include "idt.h"
#include "interrupts.h"

extern void custom_isrs_init();
extern void keyboard_init();

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Function to convert uint32_t to hex string
void uint_to_hex(uint32_t num, char* str) {
    const char hex_chars[] = "0123456789ABCDEF";
    for(int i = 7; i >= 0; i--) {
        str[i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    str[8] = '\0';
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize terminal first for output
    terminal_initialize();
    terminal_writestring("Terminal initialized\n");
   
    // Debug: Print the magic number we received
    char hex_str[9];
    uint_to_hex(magic, hex_str);
    terminal_writestring("Received magic number: 0x");
    terminal_writestring(hex_str);
    terminal_writestring("\n");
   
    // Debug: Print the expected magic number
    uint_to_hex(MULTIBOOT2_BOOTLOADER_MAGIC, hex_str);
    terminal_writestring("Expected magic number: 0x");
    terminal_writestring(hex_str);
    terminal_writestring("\n");
   
    // Check multiboot magic number
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        terminal_writestring("Invalid multiboot2 magic number!\n");
        return -1;
    }
   
    // Print Hello World (explicit requirement)
    terminal_write_colored("Hello?\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
   
    terminal_write_colored("Hello\n", VGA_COLOR_RED, VGA_COLOR_BLACK);
    terminal_write_colored("Hello\n", VGA_COLOR_BROWN, VGA_COLOR_BLACK);
    terminal_write_colored("Hello...\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
    terminal_write_colored("Is there anybody in there?\n", VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_write_colored("Just nod if you can hear me\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    terminal_write_colored("Is there anyone home?\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    terminal_writestring("Hello world\n");
    
    // Initialize GDT
    terminal_writestring("Initializing GDT...\n");
    gdt_init();
    terminal_writestring("GDT initialized successfully!\n");
    
    terminal_writestring("Initializing IDT...\n");
    idt_init();
    terminal_writestring("IDT initialized successfully!\n");
   
    terminal_writestring("Kernel initialization complete!\n");
    
    terminal_writestring("Initializing IRQ...\n");
    init_irq();
    terminal_writestring("IRQ initialized successfully!\n");
    
    // Add the following code to initialize custom ISRs and keyboard
    
    // Initialize our custom ISRs
    terminal_writestring("Initializing custom ISRs...\n");
    custom_isrs_init();
    
    // Initialize keyboard handler
    terminal_writestring("Initializing keyboard...\n");
    keyboard_init();
    
    // Test our custom ISRs
    terminal_writestring("Testing custom interrupts...\n");
    asm volatile("int $40");
    asm volatile("int $41");
    asm volatile("int $42");
    
    terminal_writestring("\nSetup complete! Keyboard is now active.\n");
    
    // Main loop - do nothing but respond to interrupts
for(;;) {
    /*
    if (inb(0x64) & 0x01) {
        uint8_t scancode = inb(0x60);
        terminal_write_colored("Direct KB: ", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
        
        char hex_str[5] = "0x00\0";
        hex_str[2] = "0123456789ABCDEF"[(scancode >> 4) & 0xF];
        hex_str[3] = "0123456789ABCDEF"[scancode & 0xF];
        terminal_write_colored(hex_str, VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
        terminal_writestring("\n");
    }
    */
    
    // Just wait for interrupts
    asm volatile("hlt");
}}