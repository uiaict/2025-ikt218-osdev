
#include "gdt.h" 
#include "idt.h" 
#include "printf.h" 
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdint.h"
#include <multiboot2.h>
#include "kernel/memory.h"
#include "kernel/pit.h"

extern uint32_t end;

// extern uint32_t end; // Task 1 - define

int main(uint32_t magic, struct multiboot_info *mb_info_addr) {

    init_gdt();
    init_idt();

    init_kernel_memory(&end); // Initialize kernel memory management

    init_paging(); // Initialize paging

    print_memory_layout(); // Print the current memory layout

    init_pit();

    asm volatile("sti");
  
  // Clear previous buffer
    while (inb(0x64) & 0x01) {
        inb(0x60); 
    }
  
    int counter = 0;

    // teste sleeping

    printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
    sleep_busy(1000);
    printf("[%d]: Slept using busy-waiting.\n", counter++);

    printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
    sleep_interrupt(1000);
    printf("[%d]: Slept using interrupts.\n", counter++);

    // malloc something and print the address
    printf("malloc(0x1000) = 0x%x\n", malloc(0x1000));
  
    printf("Hello, Aquila!\n");
    printf("aquila: ");


    while (1) {
        asm volatile("hlt"); 
    }
    return 0;
}