#include <multiboot2.h>
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "libc/monitor.h"
#include "keyboard/keyboard.h"
#include "gdt/descriptor_tables.h"
#include "PIT/pit.h"
#include "memory/memory.h"
#include "memory/paging.h"
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

    monitor_write_color(3, "Testing interrupt 1 (int $0x1)\n");
    asm volatile ("int $0x1");

    monitor_write_color(3, "Testing interrupt 2 (int $0x3)\n");
    asm volatile ("int $0x3");

    init_kernel_memory(&end);
    init_paging();
    monitor_write("\n");

    // Memory test
    monitor_write_color(2, "Memory Test:\n");
    monitor_write_color(6, "  Allocating 20 bytes...\n");
    void* mem1 = malloc(20);
    
    monitor_write_color(6, "  Allocating 50 bytes...\n");
    void* mem2 = malloc(50);
    
    print_memory_layout();

    // Timer test
    monitor_write_color(5, "\nInitializing PIT (Programmable Interval Timer)...\n");
    init_pit();

    monitor_write_color(5, "Sleeping for 7 seconds using interrupt-based sleep...\n");
    sleep_interrupt(7000);

    monitor_clear();

    // OS initialization complete
    monitor_write_color(10, "Operating system initialized!\n");

    // Keyboard and shell
    init_keyboard();

    monitor_write_color(11, "Launching shell...\n");
    init_shell();
    run_shell();

    // Shell exited
    monitor_write_color(4, "\n Kernel has stopped. System halted.\n");

    while (1);

    return 0;
}
