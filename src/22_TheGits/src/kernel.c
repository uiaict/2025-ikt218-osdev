#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "libc/stdarg.h"
#include "libc/gdt.h" 
#include "libc/scrn.h"
#include "libc/idt.h"
#include "libc/isr_handlers.h"
#include "libc/irq.h"
#include "memory/memory.h"
#include "pit/pit.h"
#include "audio/song.h"
#include "audio/player.h"
#include "audio/tracks.h"
#include "game/wordgame.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

extern uint32_t end; // End of kernel memory

void play_music() {
    Song songs[] = {
        {music_1, sizeof(music_1) / sizeof(Note)},
        // Flere sanger kan legges til her
    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song);

    SongPlayer* player = create_song_player();

    while (1) {
        for (uint32_t i = 0; i < n_songs; i++) {
            printf("Playing Song...\n");
            player->play_song(&songs[i]);
            printf("Finished playing the song.\n");
        }
    }

    free(player); //<- never reaches this point due to infinite loop, but good practice to have here
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
   
    // === SYSTEMINIT ===
    init_gdt();
    // timer_phase(100); denne erstattes av init_pit();
    remap_pic();
    init_idt();
    init_irq();
    init_kernel_memory(&end);
    init_paging();
    init_pit();

    __asm__ volatile ("sti"); // Aktiver interrupts

    // === SKJERMSTART ===
    printf("Hello, World!\n");

    // === SPILL ===
    start_word_game(); // Spill starter her – fullfør først

    // === TEST MALLOC ===
    void* test = malloc(100);
    printf("Malloc adresse: 0x%x\n", (uint32_t)test);


    // === MUSIKK ===
    play_music(); // Starter musikk etter spillet

    /* int counter = 0;
    while(true){
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);
    }; */

    // === HVILE ===
    while (1) {
        __asm__ volatile ("hlt");
    }

    return 0;
}
