#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include "libc/system.h"

#include <multiboot2.h>
#include "libc/gdt.h"
#include "libc/idt.h"
#include "pit.h"
#include "interrupts.h"
#include "memory/memory.h"
#include "Music/frequencies.h"
#include "Music/song.h"
#include "libc/matrix_rain.h"

#include "libc/stdlib.h"
#include "libc/stddef.h"

// Custom global new/delete operators
void* operator new(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) panic("Memory allocation failed");
    return ptr;
}
void* operator new[](size_t size) {
    void* ptr = malloc(size);
    if (!ptr) panic("Memory allocation failed");
    return ptr;
}
void operator delete(void* ptr) noexcept { free(ptr); }
void operator delete[](void* ptr) noexcept { free(ptr); }
void operator delete(void* ptr, size_t) noexcept { free(ptr); }
void operator delete[](void* ptr, size_t) noexcept { free(ptr); }

// Forward declaration (already implemented in song.cpp)
//void play_song_impl(Song* song);


// Music playback logic
void play_music() {
    Song songs[] = {
        {starwars_theme, sizeof(starwars_theme) / sizeof(Note)},
        {music_1, sizeof(music_1) / sizeof(Note)}
    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song);

    SongPlayer* player = create_song_player();

    while (true) {
        for (uint32_t i = 0; i < n_songs; i++) {
            printf("Playing song...\n");
            player->play_song(&songs[i]);
            printf("Finished playing the song.\n");
        }
    }
}


// Kernel main
extern "C" int kernel_main() {
    

    asm volatile("sti");

    printf("Kernel initialized successfully\n");
    sleep_interrupt(1000);
    play_music();  // <- starts the loop and never returns
   

    while (true) {
        asm volatile("hlt");
    }

    return 0;
}

// Multiboot entry
extern "C" __attribute__((noreturn)) void kmain(uint32_t magic, uint32_t* mb_info) {
    (void)magic;
    init_kernel_memory(mb_info);
    init_paging();

    kernel_main();
    

    while (true) asm volatile("hlt");
}
