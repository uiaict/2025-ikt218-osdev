
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

int main(uint32_t magic, struct multiboot_info *mb_info_addr) {

    init_gdt();
    init_idt();

    init_kernel_memory(&end); // Initialize kernel memory management

    init_paging(); // Initialize paging

    print_memory_layout(); // Print the current memory layout

    init_pit();

    asm volatile("sti");

    int counter = 0;

    // teste sleeping

    for (int i = 0; i < 2; i++) {
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);
    
        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);
    }

    while (1) {
        asm volatile("hlt"); 
    }
    return 0;
}