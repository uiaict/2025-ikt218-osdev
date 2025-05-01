#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "keyboard/keyboard.h"
#include "gdt/descriptor_tables.h"
#include "PIT/timer.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "libc/monitor.h"
#include "PIT/pit.h"
#include <multiboot2.h>
#include "song/song.h"
#include "song/SongPlayer.h"
#include "ui/shell.h"

extern uint32_t end;




struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize hardware and OS components
    init_descriptor_tables();
    asm volatile ("int $0x1");
    asm volatile ("int $0x3");
    init_keyboard();
    init_kernel_memory(&end);
    init_paging();
   
    // Memory test
    void* mem1 = malloc(20);
    void* mem2 = malloc(50);
    print_memory_layout();
    
    // Timer test
    init_pit();
    // sleep_interrupt(1000);
    // monitor_write("Slept 1 second!\n");
    // sleep_busy(500);
    // monitor_write("Slept 0.5 second!\n");
    
    // OS initialization complete
    monitor_write("Operating system initialized!\n");
    
    // Initialize and run the shell
    init_shell();
    run_shell();
    
    // This point is reached when shell exits
    monitor_write("\nKernel has stopped. System halted.\n");
    
    // Keep the system running
    while (1);
    
    return 0;
}