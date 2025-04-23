#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Function to halt system execution
static inline void panic(const char* message) {
    // Print message and halt
    printf("PANIC: %s\n", message);
    for(;;);
}

#endif // SYSTEM_H