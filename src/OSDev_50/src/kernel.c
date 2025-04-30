
#include <multiboot2.h>
#include "monitor.h"
#include "gdt.h"
#include "descriptor_tables.h"
#include "interrupts.h"
#include "memory.h"
#include "pit.h"
#include "song.h"
#include "libc/stdio.h"
#include "matrix_rain.h"

// Structure to hold multiboot information.
struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Symbol from linker script marking end of kernel image
extern uint32_t end;

// Entry point called by the multiboot2 bootloader
void kernel_main(uint32_t magic, struct multiboot_tag* tags) {
    

    monitor_initialize();
    init_gdt();
    init_idt();
    init_irq();

    rain_init();
    
    printf("Hello, World!\n");

    // Memory management
    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();

    // PIT timer for ms ticks
    init_pit();
    asm volatile("sti");  // enable interrupts

    // In your init code, before you do anything else:
    play_sound(440);       // A4 tone
    sleep_busy(500);       // half a second
    disable_speaker();     // or stop_sound()

    // Play predefined song
    Song song = { music_1, music_1_length };
    play_song_impl(&song);

    rain_enabled = 1;   // START Matrix rain now!!

    // Halt forever
    for (;;) asm volatile("hlt");
}