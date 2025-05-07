#include "kernelUtils.h"
#include "terminal.h"  
#include "libc/stdint.h"

// PANIC FUNCTION 
void panic(const char *message)
{
    // Print the panic message
    printf("\nKernel panic: %s\n", message);

    // Halt the CPU
    asm volatile("cli; hlt");

    while (1) { }
}
