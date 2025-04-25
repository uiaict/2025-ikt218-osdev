#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"

#include "kernel_memory.h"
#include "pit.h"
#include "paging.h"
#include "interrupt.h"
#include "io.h"



int kernel_main() {
    set_color(0x0B); // light cyan text
    puts("=== Entered kernel_main ===\n");
    uint32_t counter = 0;


    // Allocate some memory
    void* block1 = malloc(4096);
    void* block2 = malloc(8192);
    void* block3 = malloc(1024);

    if (block1 && block2 && block3) {
        printf("Memory blocks allocated successfully!\n", 0);
    } else {
        printf("Memory allocation failed!\n", 0);
    }

        // Infinite testing loop
    while (true) {
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000); // Sleep 1000 milliseconds (1 second)
        printf("[%d]: Slept using busy-waiting.\n", counter++);
        
        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000); // Sleep 1000 milliseconds (1 second)
        printf("[%d]: Slept using interrupts.\n", counter++);
    }

    return 0;
}
