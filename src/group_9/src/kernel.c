#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "keyboard.h"
#include "terminal.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "memory.h"
#include "pit.h"
#include "speaker.h"
#include "song.h"
#include "adventure.h"


extern uint32_t end;

// Test function for playing music
void test_music_player() {
    Song test_song = { music_1, sizeof(music_1) / sizeof(Note) };
    SongPlayer* player = create_song_player();
    terminal_printf("\n[SONG TEST] Playing test song...\n");
    player->play_song(&test_song);
    terminal_printf("[SONG TEST] Finished playing the song.\n");
    free(player);

}

int kernel_main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    (void)magic;
    (void)mb_info_addr;

    terminal_initialize();
    terminal_setcolor(0x0A);
    terminal_printf("[INFO] Terminal initialized.\n");

    gdt_install();
    idt_install();
    keyboard_install();
    terminal_printf("[OK] GDT, IDT, and Keyboard IRQ initialized.\n");

    init_kernel_memory(&end);
    terminal_printf("[INFO] Kernel heap starts at 0x%x\n", (uint32_t)&end);

    void* block1 = malloc(32);
    terminal_printf("[INFO] First allocation at 0x%x\n", (uint32_t)block1);
    print_memory_layout();

    init_paging();
    terminal_printf("[OK] Paging enabled.\n");

    init_pit();
    terminal_printf("[OK] PIT initialized.\n");

    terminal_printf("Hello from Group 9!\n");

    test_music_player(); // 🔊 Call music test here

    start_adventure();

    while (1) {
        __asm__ __volatile__("hlt");
    }
}
