#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "arch/i386/GDT/gdt.h"
#include "arch/i386/interrupts/idt.h"
#include "drivers/VGA/vga.h"
#include "arch/i386/interrupts/keyboard.h"
#include "../memory/memory.h"
#include "../PIT/pit.h"
#include "drivers/audio/song.h" 

extern uint32_t end;

// Function declarations
void test_music_player();

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    initGdt();
    initIdt();

    initKeyboard();

    Reset();
    show_animation();
    print("OSDev_75 Booted Successfully!\r\n");

    print("Initializing memory management...\n");
    init_kernel_memory(&end);
    
    print("Initializing paging...\n");
    init_paging();
    
    print("Memory layout:\n");
    print_memory_layout();

    print("Initializing PIT...\n");
    init_pit();

    print("Testing memory allocation...\n");
    void* some_memory = malloc(12345);
    void* memory2 = malloc(54321);
    void* memory3 = malloc(13331);
    char* memory4 = malloc(1000);
    print("Memory allocation test completed.\n");

    print("Triggering ISR1 (Debug)...\n");
    asm("int $0x1");  

    print("Triggering ISR2 (NMI)...\n");
    asm("int $0x2"); 

    print("Triggering ISR3 (Breakpoint)...\n");
    asm("int $0x3");  
    print("Triggering ISR128 (Syscall)...\n");
    asm("int $0x80"); 

    uint32_t counter = 0;
    print("Testing PIT sleep functions...\n");
    
    for (int i = 0; i < 3; i++) {
        print("Sleeping with busy-waiting (HIGH CPU)...\n");
        sleep_busy(1000);
        print("Slept using busy-waiting.\n");
        counter++;

        print("Sleeping with interrupts (LOW CPU)...\n");
        sleep_interrupt(1000);
        print("Slept using interrupts.\n");
        counter++;
    }

    // Test the music player
    print("Testing music player...\n");
    test_music_player();

    for (;;) {
        __asm__ __volatile__("hlt");
    }
    return 0;
}

// Function to test the music player
void test_music_player() {
    // Create song structures from the predefined note arrays
    Song songs[] = {
        {music_1, sizeof(music_1) / sizeof(Note)},
        {music_2, sizeof(music_2) / sizeof(Note)},
        {music_3, sizeof(music_3) / sizeof(Note)}
    };
    
    // Calculate the number of songs
    uint32_t n_songs = sizeof(songs) / sizeof(Song);
    
    // Create a song player
    SongPlayer* player = create_song_player();
    
    // Play each song in the array
    for (uint32_t i = 0; i < n_songs; i++) {
        print("Playing Song...\n");
        player->play_song(&songs[i]);
        print("Finished playing the song.\n");
        
        // Add a short pause between songs
        sleep_interrupt(1000);
    }
    
    print("Music player test completed.\n");
}