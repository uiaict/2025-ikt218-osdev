#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
//#include "libc/stdio.h"
#include <multiboot2.h>
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "terminal.h"
#include "io.h"
#include "keyboard.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Test keyboard handler function
void keyboard_handler(registers_t regs) {
    uint8_t scancode = inb(0x60);
    terminal_write("Keyboard input detected!\n");
}

// Forward declaration of kernel_main from kernel.cpp
int kernel_main();

#define MULTIBOOT2_MAGIC 0x36d76289
int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize GDT
    gdt_init();
   
    // Initialize terminal
    terminal_init();
   
    // Initialize IDT
    init_idt();

    // Initialize keyboard
    initKeyboard(); 

    changeBackgroundColor(vgaColorDarkGrey); 
    changeTextColor(vgaColorWhite); 
   
    
   
    // Print a test message
    terminal_write("Hello World\n");
   
    // Test interrupt
    asm volatile("int $0x0");
   
    // Return kernel_main (from kernel.cpp)
    return kernel_main();
}