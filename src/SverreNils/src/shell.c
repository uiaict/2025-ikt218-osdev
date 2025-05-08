#include "printf.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>              
#include "song/song.h"
#include "song/note.h"
#include "piano.h"
#include "pit.h"
#include "devices/keyboard.h"


extern Note music_1[];
extern const size_t music_1_len;
extern void play_song_impl(Song* song);

void shell_prompt() {
    printf("UiAOS> ");
}


static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}


void shutdown() {
    outw(0x604, 0x2000);
}

void run_sleep_demo() {
    int counter = 0;
    for (int i = 0; i < 3; i++) {
        printf("[%d]: Sleeping busy...\n", counter);
        sleep_busy(1000);
        printf("[%d]: Done busy.\n", counter++);

        printf("[%d]: Sleeping interrupt...\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Done interrupt.\n", counter++);
    }
}

void shell_handle_input(const char* input) {
    if (strcmp(input, "help") == 0) {
        printf("Tilgjengelige kommandoer:\n");
        printf(" - help\n - clear\n - echo [tekst]\n - shutdown\n - play\n - piano\n");
        printf(" - sleep\n");
    } else if (strncmp(input, "echo ", 5) == 0) {
        printf("%s\n", input + 5);
    } else if (strcmp(input, "clear") == 0) {
        for (int i = 0; i < 80 * 25; i++)
            ((uint16_t*)0xB8000)[i] = 0x0720;
    } else if (strcmp(input, "shutdown") == 0) {
        printf("Shutting down...\n");
        shutdown();
    } else if (strcmp(input, "play") == 0) {
        Song song = {music_1, music_1_len};
        printf("🎵 Spiller musikk...\n");
        play_song_impl(&song);
        printf("✅ Ferdig!\n");
    } else if (strcmp(input, "piano") == 0) {
        printf("🎹 Starter piano...\n");
        init_piano();
    } else if (strcmp(input, "sleep") == 0) {
        run_sleep_demo();
        __asm__ volatile("sti");
        restore_keyboard_handler();
        reset_input_buffer();
        return;  
    } else {
        printf("Ukjent kommando: %s\n", input);
    }
}
