#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "libc/monitor.h"
#include "libc/memory/memory.h"
#include "libc/pit.h"
#include <multiboot2.h>
#include "arch/i386/gdt/gdt.h"
#include "arch/i386/idt/idt.h"

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    
    //Initialize gdt from gdt.h
    init_gdt();

    //Initialize idt from idt.h
    init_idt();

    //Initialize the keyboard
    init_keyboard();

    //Enable interrupts
    __asm__ volatile ("sti");

    //Interupt 0 test 
    asm("int $0x0");

    init_kernel_memory(&end); // <------ THIS IS PART OF THE ASSIGNMENT

    init_paging();

    print_memory_layout();
    
    init_pit(); // <------ THIS IS PART OF THE ASSIGNMENT

    uint32_t counter = 0;

    while (true) {
        monitor_write("[");
        monitor_write_dec(counter);
        monitor_write("]: Sleeping with busy-waiting (HIGH CPU).\n");
    
        sleep_busy(1000);
    
        monitor_write("[");
        monitor_write_dec(counter);
        monitor_write("]: Slept using busy-waiting.\n");
    
        counter++;
    
        monitor_write("[");
        monitor_write_dec(counter);
        monitor_write("]: Sleeping with interrupts (LOW CPU).\n");
    
        sleep_interrupt(1000);
    
        monitor_write("[");
        monitor_write_dec(counter);
        monitor_write("]: Slept using interrupts.\n");
    
        counter++;
    }
    
    //infinite loop to keep the kernel running
    while (1) {
        __asm__ volatile ("hlt");
    }

    return 0;

}