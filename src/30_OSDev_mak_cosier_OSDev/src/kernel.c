// kernel.c
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/multiboot2.h"
#include "libc/teminal.h"  // For terminal functions
#include "libc/gdt.h"
#include "libc/idt.h"
#include "libc/keyboard.h"
#include "libc/vga.h"
#include "libc/io.h"
#include "libc/pit.h"
#include "libc/memory.h"

// Declaration for the kernel end symbol defined in linker.ld
extern uint32_t end;

struct multiboot_info 
{
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) 
{
    // Initialize base systems
    init_gdt();
    init_idt();
    initKeyboard();    
    __asm__ volatile ("sti");
    
    // --------------------------------
    // Memory Management Initialization
    // --------------------------------
    
    // Initialize the kernel memory manager using the end address of the kernel.
    init_kernel_memory((uint32_t)&end);
    
    // Initialize paging for memory management.
    paging_init();

    // Print the memory layout to verify the memory manager's state.
    print_memory_layout();

    // Test memory allocation:
    void* mem1 = malloc(1000);
    void* mem2 = malloc(500);
    kprint("Allocated memory blocks at %x and %x\n", mem1, mem2);

    // -----------------------------
    // PIT (Programmable Interval Timer) Initialization
    // -----------------------------
    
    // Initialize the PIT for timer interrupts.
    init_pit();

    // Test the sleep functions with a counter
    int counter = 0;
    
    kprint("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
    sleep_busy(1000);
    kprint("[%d]: Slept using busy-waiting.\n", counter++);

    kprint("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
    sleep_interrupt(1000);
    kprint("[%d]: Slept using interrupts.\n", counter++);

    // Continue running with a loop to demonstrate PIT functionality
    while(1) {
        kprint("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        kprint("[%d]: Slept using busy-waiting.\n", counter++);

        kprint("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        kprint("[%d]: Slept using interrupts.\n", counter++);
    }

    return 0;
}
