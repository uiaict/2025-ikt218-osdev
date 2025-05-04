#include "kernelUtils.h"
#include "terminal.h"  
#include "libc/stdint.h"

// Kernel panic function
void panic(const char *message)
{
    // Print the panic message
    printf("\nKernel panic: %s\n", message);

    // Halt the CPU
    asm volatile("cli; hlt");

    // This will never be reached, but keeps compilers happy
    while (1) { }
}
