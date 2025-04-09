#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "gdt.h"
#include "terminal.h"
#include "common.h"
#include "idt.h"
#include "interrupts.h"
#include "memory.h"
#include "pit.h"
#include "song.h"
#include "keyboard.h"  // Add this to fix keyboard_get_scancode warning
#include "menu.h"

extern void custom_isrs_init();
extern void keyboard_init();

// From linker.ld - marks end of kernel code
extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Convert a number to hex string
void uint_to_hex(uint32_t num, char* str) {
    const char hex_chars[] = "0123456789ABCDEF";
    for(int i = 7; i >= 0; i--) {
        str[i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    str[8] = '\0';
}

// Convert integer to string
void int_to_str(int num, char* str) {
    int i = 0;
    bool is_negative = false;
    
    // Handle 0 case
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    // Handle negative numbers
    if (num < 0) {
        is_negative = true;
        num = -num;
    }
    
    // Convert digits
    while (num != 0) {
        str[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    // Add negative sign if needed
    if (is_negative) {
        str[i++] = '-';
    }
    
    str[i] = '\0';
    
    // Reverse the string
    int j;
    for (j = 0; j < i/2; j++) {
        char temp = str[j];
        str[j] = str[i-j-1];
        str[i-j-1] = temp;
    }
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize terminal for output
    terminal_initialize();
    terminal_writestring("Terminal initialized\n");
   
    // Print magic number for debug
    char hex_str[9];
    uint_to_hex(magic, hex_str);
    terminal_writestring("Magic number: 0x");
    terminal_writestring(hex_str);
    terminal_writestring("\n");
   
    // Check multiboot magic
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        terminal_writestring("Invalid multiboot2 magic number!\n");
        return -1;
    }
   
    // Show boot splash
    terminal_write_colored("=================================\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    terminal_write_colored("   Welcome to 31_inefficientOS   \n", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_write_colored("=================================\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    terminal_writestring("\n");
   
    // Initialize GDT
    terminal_writestring("Initializing GDT...\n");
    gdt_init();
    terminal_writestring("GDT initialized!\n");
   
    // Initialize IDT
    terminal_writestring("Initializing IDT...\n");
    idt_init();
    terminal_writestring("IDT initialized!\n");
   
    // Initialize IRQs
    terminal_writestring("Initializing IRQ...\n");
    init_irq();
    terminal_writestring("IRQ initialized!\n");
   
    // Initialize custom ISRs
    terminal_writestring("Initializing custom ISRs...\n");
    custom_isrs_init();
   
    // Init keyboard
    terminal_writestring("Initializing keyboard...\n");
    keyboard_init();
    
    // Initialize memory management
    terminal_writestring("Initializing kernel memory...\n");
    init_kernel_memory(&end);
    
    terminal_writestring("Initializing paging...\n");
    init_paging();
    
    // Initialize PIT
    terminal_writestring("Initializing PIT...\n");
    init_pit();
    
    // Initialize menu system
    terminal_writestring("Initializing menu system...\n");
    menu_init();
    
    terminal_write_colored("\nAll systems initialized!\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("Starting menu system in 3 seconds...\n");
    
    // Wait a moment before showing the menu
    sleep_interrupt(3000);
    
    // Run the main menu
    main_menu_run();
    
    // If we get here, the menu has been exited (ESC on the main menu)
    terminal_initialize();
    terminal_write_colored("\n31_inefficientOS is now running in direct mode.\n", 
                          VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
    terminal_writestring("Press any key to restart the menu system.\n\n");
    
    // Main loop that allows returning to the menu
    while(1) {
        uint8_t scancode = keyboard_get_scancode();
        if (scancode != 0) {
            main_menu_run();
            
            terminal_initialize();
            terminal_write_colored("\n31_inefficientOS is now running in direct mode.\n", 
                                  VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
            terminal_writestring("Press any key to restart the menu system.\n\n");
        }
        
        // Small delay to prevent CPU hogging
        sleep_busy(10);
    }
    
    return 0;
}