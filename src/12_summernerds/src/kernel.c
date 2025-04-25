#include <libc/stdint.h>
#include <libc/stddef.h>
#include "libc/stdbool.h"
#include <../src/screen.h>
#include "../src/arch/i386/keyboard.h"
// #include "../src/arch/i386/print.h"
#include "../src/common.h"
#include "../src/arch/i386/gdt.h"
// #include "../src/arch/i386/IDT.h"
// #include "../src/arch/i386/ISR.h"
#include "../src/arch/i386/interuptRegister.h"
#include "../src/arch/i386/monitor.h"
#include "../include/kernel/pit.h"
#include "kernel/memory.h"
// #include <kheap.h>
// #include <paging.h>

extern uint32_t end; // Linker symbol marking the end of kernel

int main(uint32_t magic, uint32_t mb_info_addr)
{

    // initializing basic systems
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
    void *mem1 = malloc(12345);
    void *mem2 = malloc(54321);
    void *mem3 = malloc(13331);

    // print if we get any problem with allocating memory
    printf("Allocated memory blocks at: %p, %p, %p\n", mem1, mem2, mem3);

    // Test PIT sleep
    int counter = 0;
     while (true)
    {
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU)...\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU)...\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);
    }

    while(1) {}

    // Usually shouldnt get here, since it then quits kernel main.
    return 0;
}
