#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "gdt.h"
#include "terminal.h"

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
    terminal_write_colored("Terminal initialized\n", VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    
    // Debug: Print the magic number we received
    char hex_str[9];
    uint_to_hex(magic, hex_str);
    terminal_write_colored("Received magic number: 0x", VGA_COLOR_BLUE, VGA_COLOR_BLACK);
    terminal_write_colored(hex_str, VGA_COLOR_CYAN, VGA_COLOR_BLACK);
    terminal_writestring("\n");
   
    // Debug: Print the expected magic number
    uint_to_hex(MULTIBOOT2_BOOTLOADER_MAGIC, hex_str);
    terminal_write_colored("Expected magic number: 0x", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    terminal_write_colored(hex_str, VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
    terminal_writestring("\n");
    
    // Check multiboot magic number
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        terminal_write_colored("Invalid multiboot2 magic number!\n", VGA_COLOR_RED, VGA_COLOR_BLACK);
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
    terminal_write_colored("Hello world\n", VGA_COLOR_BLACK, VGA_COLOR_WHITE);



    // Initialize GDT
    terminal_write_colored("Initializing GDT...\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    gdt_init();
    terminal_write_colored("GDT initialized successfully!\n", VGA_COLOR_BROWN, VGA_COLOR_BLACK);
   
    terminal_write_colored("Kernel initialization complete!\n", VGA_COLOR_GREEN, VGA_COLOR_BLACK);
   
    while(1) {
        asm volatile("hlt");
    }
   
    return 0;

}
