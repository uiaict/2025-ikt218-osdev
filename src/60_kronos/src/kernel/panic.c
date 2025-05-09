#include "kernel/panic.h"
#include "libc/stdio.h"
#include "libc/string.h"

void panic(const char* message) {
    // Disable interrupts
    asm volatile("cli");
    
    printf("\n\n");
    printf("****************************\n");
    printf("*        KERNEL PANIC      *\n");
    printf("****************************\n");
    printf("Error: %s\n", message);
    printf("System halted.\n");
    
    // Infinite loop with CPU halt to stop execution
    for (;;) {
        asm volatile("hlt");
    }
}