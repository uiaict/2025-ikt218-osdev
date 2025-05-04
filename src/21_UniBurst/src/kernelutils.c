#include "kernelutils.h"
#include "libc/stdio.h"
#include "libc/stdint.h"

// Kernel panic function
void panic(const char *message)
{
    // Print the panic message
    printf("Kernel panic: %s\n", message);

    // Halt the CPU
    asm volatile("cli; hlt");
}



void debug_print_address(uint32_t address) {
    char buffer[16]; 
    
   
    buffer[0] = '0';
    buffer[1] = 'x';
    
    const char* digits = "0123456789ABCDEF";
    
   
    for (int i = 0; i < 8; i++) {
        int digit = (address >> (28 - i * 4)) & 0xF;
        buffer[2 + i] = digits[digit];
    }
    
    buffer[10] = '\0';
    
   
    printf(buffer);
}