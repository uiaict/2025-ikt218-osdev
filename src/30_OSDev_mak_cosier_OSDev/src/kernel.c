#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/multiboot2.h"
#include "libc/teminal.h"
#include "libc/gdt.h"
#include "libc/idt.h"

#include "libc/keyboard.h"
#include "libc/vga.h"
#include "libc/io.h"

//#include "../include/libc/pit.h"
//#include "../include/libc/memory.h"

//#include "../include/libc/memory.h"  
//#include "../include/libc/pit.h"  

// Declaration for the kernel end symbol defined in linker.ld
//extern uint32_t end;

struct multiboot_info 
{
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) 
{

    //kprint("Hello, World!\n")

    init_gdt();
    init_idt();
    initKeyboard();    
    //int i = 5/0;
    __asm__ volatile ("sti");
    //int i = 1/0;
    
    //asm volatile ("int $0x03");

    //print_number(12345);s
    //printf("\n");
    //print_number(-9876); 
    //printf("\n"); 
    //print_number(0);

    //printf("Keyboard handler called\n");

    
    
    
/*
    // --------------------------------
    // Memory Management Initialization
    // --------------------------------
    
    // Initialize the kernel memory manager using the address of the end symbol.
    //init_kernel_memory(&end);

    // Set up paging for virtual memory management.
    init_paging();

    // Print the memory layout to verify the memory manager's state.
    print_memory_layout();

    // Test memory allocation:
    void* mem1 = malloc(1000);
    void* mem2 = malloc(500);
    printf("Allocated memory blocks at %p and %p\n", mem1, mem2);

    // Test the new operator (if applicable in your C++ integration)
    char* testNew = new char[50]();
    printf("Allocated memory using new at %p\n", testNew);

    // -----------------------------
    // PIT (Programmable Interval Timer) Initialization
    // -----------------------------
    
    // Initialize the PIT for timer interrupts.
    init_pit();

    // For test sleep functions:
    sleep_busy(1000);      // Busy-wait sleep for 1000 milliseconds
    sleep_interrupt(1000); // Sleep with interrupts for 1000 milliseconds

    // Enter the main loop (or continue with further kernel initialization)
*/


    while(1);
    return 0;
}

