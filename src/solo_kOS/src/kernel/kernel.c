//kernel.c
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "libc/stdio.h"

#include "kernel/gdt.h"
#include "kernel/interrupts.h"
#include "kernel/pit.h"

#include "common/monitor.h"  
#include "common/io.h"  

#include "kernel/memory.h"

#include "libc/randomizer.h"

#include <multiboot2.h>

volatile int last_key = 0;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();

// End of kernel image decalerd in linker file
extern uint32_t end;

void wait_for_keypress() {

    volatile bool key_pressed = false;

    void temp_key_callback(registers_t* regs, void* context) {
        key_pressed = true;
    }

    register_irq_handler(1, temp_key_callback, NULL);

    while (!key_pressed) {
        asm volatile("sti");
        asm volatile("hlt"); // Halt CPU until next interrupt
    }
    unregister_irq_handler(1);
}

int kernel_main_c(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize the monitor
    monitor_init(); 

    // Initialize GDT
    printf("Initializing GDT...\n");
    init_gdt(); 

    // Initialize IDT
    printf("Initializing IDT...\n");
    init_idt();

    //initialize interrupts
    printf("Initializing interrupts...\n");
    init_irq();

    // Initialize kernel memory management
    printf("initilazing kernel memory...\n");
    init_kernel_memory(&end); 

    // Initialize paging
    printf("Initializing paging...\n");
    init_paging();

    // Initialize PIT (Programmable Interval Timer)
    printf("Initializing PIT...\n");
    init_pit();

    // Initialize random number generator
    printf("Initializing random number generator...\n");
    rand_init();

    // Print memory layout
    print_memory_layout();

    // Initialize interrupt functions
    printf("Initializing interrupt functions...\n");
    init_interrupt_functions();
    
    printf("All kernel functionality is good to \n");
    
    monitor_clear(); // Clear the monitor screen
    // Call cpp kernel_main (defined in kernel.cpp)
    return kernel_main();
}