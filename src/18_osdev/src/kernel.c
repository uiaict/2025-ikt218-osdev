#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "keyboard/keyboard.h"
#include "gdt/descriptor_tables.h"
#include "PIT/timer.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "libc/monitor.h"
#include "PIT/pit.h"
#include <multiboot2.h>

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    init_descriptor_tables();

    // asm volatile ("int $0x1");
    // asm volatile ("int $0x3");

    // init_timer(50); used this to check that irq works
    init_keyboard();

    init_kernel_memory(&end);
    init_paging();
    

    void* mem1 = malloc(20);
    void* mem2 = malloc(50);

    print_memory_layout();

    init_pit();  // Initialize timer

    sleep_interrupt(1000); // sleep 1 second using interrupts
    monitor_write("Slept 1 second!\n");

    sleep_busy(500);       // sleep 0.5 second using busy waiting
    monitor_write("Slept 0.5 second!\n");

    return 0;
}