#include "libc/stdio.h"
#include "libc/stdint.h"
#include "interrupts/desTables.h"
#include "drivers/keyboard.h"
#include "interrupts/pit.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "music/sound.h"
#include "music/song.h"
#include "music/notes.h"

extern uint32_t end;
extern char charBuffer[];
extern int bufferIndex;

extern Note music_1[];
extern size_t music_1_length;

void kmain(uint32_t magic, void* mb_info_addr) {
    printf("Hello World!\n");

    // Init OS subsystems
    initDesTables();

    initKeyboard();

    initPit();

    // Init memory systems
    init_kernel_memory(&end);

    init_paging();

    // Show memory map from multiboot info
    print_memory_layout(magic, mb_info_addr);

    // Confirm subsystems are ready
    printf("[OK] Descriptors initialized\n");
    printf("[OK] Keyboard initialized\n");
    printf("[OK] PIT initialized\n");

    // Test malloc
    void* mem1 = malloc(1234);
    void* mem2 = malloc(4321);
    printf("Allocated mem1: 0x%x, mem2: 0x%x\n", (uint32_t)mem1, (uint32_t)mem2);

    // Test PIT sleep functions
    for (int i = 0; i < 3; i++) {
        printf("[%d] Sleeping busy...\n", i);
        sleepBusy(1000);
        printf("[%d] Done busy sleep.\n", i);

        printf("[%d] Sleeping interrupt...\n", i);
        sleepInterrupt(1000);
        printf("[%d] Done interrupt sleep.\n", i);
    }


    Song song = { music_1, music_1_length };
    play_song(&song);

    // Keyboard input test
    printf("You can now type! Input will be printed live:\n");
    bufferIndex = 0;

    while (1) {
        asm volatile("hlt"); // Idle loop
    }
}
