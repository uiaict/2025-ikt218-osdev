#include "multiboot2.h"
#include "arch/idt.h"
#include "arch/isr.h"
#include "arch/irq.h"
#include "shell.h"
#include "devices/keyboard.h"
#include "arch/gdt.h"
#include "kernel_memory.h"
#include "paging.h"
#include "pit.h"

#include "printf.h"
#include "stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

#include "song/song.h"
#include "song/note.h"

extern uint32_t end; // Definert av linker.ld
extern Note music_1[];                 // Fra music_1.c
extern const size_t music_1_len;       // 🔧 lagt til
extern void play_song_impl(Song* song); // Fra song_player.c

// Enkel SongPlayer-struct hvis malloc ikke brukes
typedef struct {
    void (*play_song)(Song* song);
} SongPlayer;

SongPlayer* create_song_player() {
    static SongPlayer player;
    player.play_song = play_song_impl;
    return &player;
}

void play_music() {
    Song songs[] = {
        {music_1, music_1_len} // 🔧 riktig måte å hente lengde på
    };

    SongPlayer* player = create_song_player();

    for (size_t i = 0; i < sizeof(songs)/sizeof(Song); i++) {
        printf("🎵 Spiller sang...\n");
        player->play_song(&songs[i]);
        printf("✅ Ferdig!\n");
    }
}

void putc_raw(char c) {
    volatile char* video = (volatile char*)(0xB8000 + 160 * 23); // linje 24
    video[0] = c;
    video[1] = 0x07;
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    idt_init();
    isr_install();
    irq_install();
    init_keyboard();

    gdt_init();
    __asm__ volatile("sti"); // Aktiver maskinavbrudd

    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();
    init_pit();

    printf("Hello, Nils!\n");

    // 🎵 Spill musikk ved oppstart
    play_music();

    void* some_memory = malloc(12345);
    void* memory2 = malloc(54321);
    void* memory3 = malloc(13331);

    shell_prompt();

    int counter = 0;
    while (1) {
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);

        __asm__ volatile("hlt");
    }
}
