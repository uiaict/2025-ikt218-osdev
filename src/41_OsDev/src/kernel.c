// Simplified kernel.c without external terminal.h dependency
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <multiboot2.h>
#include <kernel/memory.h>
#include <kernel/pit.h>
#include <libc/stdio.h>
#include <song/song.h>
#include "bootinfo.h"

// Reference to the end address of the kernel
extern uint32_t end;

// Function prototypes to fix implicit declaration warnings
void terminal_initialize(void);
void gdt_install(void);
void idt_install(void);
void init_irq(void);
void print_bootinfo_memory_layout(const struct multiboot_tag_mmap *mmap_tag, uint32_t kernel_end);

int main(uint32_t magic, struct multiboot_tag* mb_info_addr) {
    // Initialize the terminal first
    terminal_initialize();
    
    // Print hello world message
    printf("Hello World\n\n");
    
    // Initialize the Global Descriptor Table
    gdt_install();

    // Initialize the Interrupt Descriptor Table
    idt_install();

    // Initialize the hardware interrupts (remap PICs)
    init_irq();

    // Initialize the kernel's memory manager
    init_kernel_memory(&end);

    // Initialize paging
    init_paging();

    // Initialize PIT
    init_pit();
    
    // Enable interrupts
    asm volatile("sti");

    // Allocate memory for key components (silently)
    void* some_memory = malloc(12345);
    void* memory2 = malloc(54321);
    void* memory3 = malloc(13331);

    // Print memory information in a more compact format
    printf("Memory allocations complete.\n");
    
    // Play the Star Wars theme using predefined notes from song.h
    Song song;
    
    // Use the Star Wars theme array defined in song.h
    song.notes = starwars_theme;
    song.length = sizeof(starwars_theme) / sizeof(Note);
    
    SongPlayer* player = create_song_player();
    player->play_song(&song);

    // Infinite loop to keep the kernel running
    for(;;) {
        asm volatile("hlt");
    }
    
    return 0;
}