#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include <gdt/gdt.h>
#include <libc/idt.h>
#include <libc/isr.h>
#include <print.h>
#include <putchar.h>
#include <libc/irq.h>
#include <keyboard.h>
#include "memory.h"  // Changed from <memory.h>
#include "libc/song.h"

extern uint32_t end; // Linker-provided symbol for end of kernel

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// A simple test handler for IRQ debugging
void test_irq_handler(registers_t regs) {
    printf("TEST IRQ HANDLER CALLED FOR IRQ %d\n", regs.int_no - 32);
    
}

SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    player->play_song = play_song_impl;
    return player;
}

void play_music() {
    // How to play music
    Song songs[] = {
        {starwars_theme, sizeof(starwars_theme) / sizeof(Note)}
    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song);

    SongPlayer* player = create_song_player();

    
    for(uint32_t i = 0; i < n_songs; i++) {
        printf("Playing Song...\n");
        player->play_song(&songs[i]);
        printf("Finished playing the song.\n");
    }
    
    free(player);
}


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize terminal for output
    terminal_initialize();

    printf("initializing kernel memory...\n");
    init_kernel_memory((uint32_t*)&end);
    printf("Kernel memory initialized\n");
    
    // Set up GDT before IDT
    printf("Initializing GDT...\n");
    gdt_install();

    // Initialize the IDT
    printf("Initializing IDT...\n");
    init_idt();
    
    printf("Initializing IRQ system...\n");
    init_irq();
    
    // Make sure interrupts are disabled while setting up handlers
    asm volatile("cli");
    
    // Initialize keyboard
    init_keyboard();
    
    // Initialize paging
    printf("Initializing paging...\n");
    init_paging();
    print_memory_layout();
    
    // Initialize PIT before enabling interrupts
    printf("Initializing PIT...\n");
    init_pit();
    
    printf("----------------------------------\n");
    printf("DEBUG: About to enable interrupts\n");
    
    // Enable interrupts after all handlers are registered
    asm volatile("sti");
    printf("Interrupts enabled\n");

    //play_music();

    int counter = 0;
    printf("Testing PIT with sleep functions...\n");
    while(1) {
        
    }

    return 0;
}