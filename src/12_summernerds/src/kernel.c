#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>
#include "screen.h"
#include "keyboard.h"
#include "kheap.h"         // for malloc(), free(), init_kernel_memory()
#include "paging.h"        // for init_paging()
#include "pit.h"           // for init_pit(), sleep functions

extern uint32_t end;       // Linker symbol marking the end of kernel

int kernel_main_c(uint32_t magic, uint32_t mb_info_addr) {

    //initializing basic systems
    monitor_initialize();
    init_gdt();
    init_idt();
    init_irq();

    // Initializing the kernel memory manager
    init_kernel_memory(&end);

    // call function to activate paging
    init_paging();

    // primt memory layout to screen
    print_memory_layout();

    // Initialize PIT (programable interval timer)
    init_pit();

    // Here we test the memory allocation
    void* mem1 = malloc(12345);
    void* mem2 = malloc(54321);
    void* mem3 = malloc(13331);

    // print if we get any problem with allocating memory
    printf("Allocated memory blocks at: %p, %p, %p\n", mem1, mem2, mem3);

    // Test PIT sleep
    int counter = 0;
    while (true) {
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU)...\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU)...\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);
    }

    // Usually shouldnt get here, since it then quits kernel main.
    return 0;
}
