#include <multiboot2.h>

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/system.h"

#include "pit.h"
#include "common.h"
#include "descriptor_tables.h"
#include "interrupts.h"
#include "monitor.h"
#include "memory/memory.h"
#include "song/frequencies.h"



// Structure to hold multiboot information.
struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Forward declaration for the C++ kernel main function.
int kernel_main();

// End of the kernel image, defined elsewhere.
extern uint32_t end;

// Main entry point for the kernel, called from boot code.
// magic: The multiboot magic number, should be MULTIBOOT2_BOOTLOADER_MAGIC.
// mb_info_addr: Pointer to the multiboot information structure.
int kernel_main_c(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize the monitor (screen output)
    monitor_initialize();
  
    // Initialize the Global Descriptor Table (GDT).
    init_gdt();

    // Initialize the Interrupt Descriptor Table (IDT).
    init_idt();

    // Initialize the hardware interrupts.
    init_irq();

    // Initialize the kernel's memory manager using the end address of the kernel.
    init_kernel_memory(&end);

    // Initialize paging for memory management.
    init_paging();

    // Print memory information.
    print_memory_layout();

    init_pit();
    asm volatile("sti");


    
    //Dette er assignment 4. kan ikke ha for assignment 5
    uint32_t counter = 0;
    // while (true) {
    // printf("[%d]: Sleeping with busy-waiting...\n", counter);
    // sleep_busy(1000);
    // printf("[%d]: Woke up from busy-waiting.\n", counter);
    // counter++;

    // printf("[%d]: Sleeping with interrupts...\n", counter);
    // sleep_interrupt(1000);
    // printf("[%d]: Woke up from interrupts.\n", counter);
    // counter++;
    // }

    // Print a hello world message.
    printf("Hello World!\n");

    // Call the C++ main function of the kernel.
    return kernel_main();
}
