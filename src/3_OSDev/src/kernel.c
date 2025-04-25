#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

#include <multiboot2.h>

#include "memory/memory.h"
#include "vga.h"
#include <libc/stdio.h>
#include "descriptor_table.h"
#include "interrupts.h"
#include "pit.h"

#include <music_player/song.h>

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

void display_ascii_logo(void) {
    printf(0x0B, "   ____   _____ _____             ____  \n");
    printf(0x0B, "  / __ \\ / ____|  __ \\           |___ \\ \n");
    printf(0x0B, " | |  | | (___ | |  | | _____   ____) | \n");
    printf(0x0B, " | |  | |\\___ \\| |  | |/ _ \\ \\ / /__ < \n");
    printf(0x0B, " | |__| |____) | |__| |  __/\\ V /___) | \n");
    printf(0x0B, "  \\____/|_____/|_____/ \\___| \\_/|____/ \n");
    printf(0x0B, "                                       \n");
    printf(0x0F, "      Operating System Development     \n");
    printf(0x07, "     UiA IKT218 Course Project Team 3  \n");
    printf(0x07, "=======================================\n");
    printf(0x0F, "\n");
}

void play_music() {
    Song songs[] = {
        {music_1, sizeof(music_1) / sizeof(Note)}
    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song);

    SongPlayer* player = create_song_player();

    while (1) {
        for(uint32_t i = 0; i < n_songs; i++) {
            printf(0x0F, "Playing song...\n");
            player->play_song(&songs[i]);
            printf(0x0F, "Finished playing song.\n");
        }
    }
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    // Display Introduction
    Reset();
    
    // Debug message before logo
    display_ascii_logo();
    
    // Initialize GDT, IDT, and IRQ handlers
    init_gdt();
    init_idt();
    init_irq();
    init_irq_handlers();
    enable_interrupts();

    // Initilise paging for memory management.
    init_kernel_memory(&end);
    init_paging();

    // Print memory information.
    print_memory_layout();

    // Initilise PIT.
    init_pit();
    
    // Print "Hello World!" to screen
    printf(0x0F, "Hello World!\n");

    // Allocate some memory
    void* some_memory = malloc(12345); 
    void* memory2 = malloc(54321); 
    void* memory3 = malloc(13331);

    print_memory_layout();

    // Test PIT
    // uint32_t counter = 0;
    // while(true){
    //     print(0x0F, "[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
    //     sleep_busy(1000);
    //     print(0x0F, "[%d]: Slept using busy-waiting.\n", counter++);

    //     print(0x0F, "[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
    //     sleep_interrupt(1000);
    //     print(0x0F, "[%d]    : Slept using interrupts.\n", counter++);
    // };

    play_music();

    while (1) {}
    return 0;

}