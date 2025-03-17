#include "../include/gdt.h"
#include "../include/terminal.h"
#include "../include/libc/stdint.h"
#include "../include/libc/stddef.h"

void kernel_main(void) {
    // Initialize terminal first
    terminal_initialize();
    
    // Initialize GDT
    init_gdt();
    
    // Print Hello World
    terminal_writestring("Hello World!\n");
}
