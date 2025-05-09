#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "keyboard.h"
#include "timer.h"
#include "kheap.h"
#include "paging.h"
#include "pcspkr.h"
#include "music_player.h"

extern uint32_t end;

static u8int music_player_active = 0;

static void handle_music_input(char key) {
    if (music_is_piano_mode()) {
        music_play_piano_note(key);
        return;
    }

    if (key >= '1' && key <= '0' + music_get_song_count()) {
        u8int song_index = key - '1';
        music_play_song(song_index);
    } else if (key == 'p' || key == 'P') {
        music_enter_piano_mode();
    } else if (key == 'q' || key == 'Q') {
        music_stop();
        monitor_write("Music stopped\n");
    } else if (key == 'l' || key == 'L') {
        monitor_write("Available songs:\n");
        for (u8int i = 0; i < music_get_song_count(); i++) {
            monitor_write_dec(i + 1);
            monitor_write(". ");
            monitor_write((char*)music_get_song_name(i));
            monitor_write("\n");
        }
    } else if (key == 'h' || key == 'H') {
        monitor_write("Music Player Commands:\n");
        monitor_write("  1-");
        monitor_write_dec(music_get_song_count());
        monitor_write(": Play a song\n");
        monitor_write("  p: Enter piano mode\n");
        monitor_write("  q: Stop playing\n");
        monitor_write("  l: List available songs\n");
        monitor_write("  h: Show this help\n");
        monitor_write("  x: Exit music player\n");
    } else if (key == 'x' || key == 'X') {
        music_player_active = 0;
        music_stop();
        music_exit_piano_mode();
        monitor_write("Exited music player. Press 'm' to return.\n");
    }
}

static void music_keyboard_handler(registers_t regs) {
    if (!music_player_active) return;

    u8int scancode = inb(0x60);

    if (!(scancode & 0x80)) {
        char c = keyboard_scancode_to_char(scancode, keyboard_is_shift_pressed());
        if (c != 0) {
            handle_music_input(c);
        }
    }
}

void kernel_main(void)
{
    monitor_clear();
    monitor_write("Initializing GDT and IDT...\n");
    init_descriptor_tables();
    enable_interrupts();

    monitor_write("Initializing kernel memory...\n");
    init_kernel_memory((u32int)&end);

    monitor_write("Initializing paging...\n");
    init_paging();

    monitor_write("Testing memory allocation...\n");
    void* some_memory = kmalloc(12345);
    void* memory2 = kmalloc(54321);
    void* memory3 = kmalloc(13331);

    monitor_write("Memory 1 address: 0x");
    monitor_write_hex((u32int)some_memory);
    monitor_write("\nMemory 2 address: 0x");
    monitor_write_hex((u32int)memory2);
    monitor_write("\nMemory 3 address: 0x");
    monitor_write_hex((u32int)memory3);
    monitor_write("\n");

    monitor_write("Initializing timer...\n");
    init_timer(50);

    monitor_write("Initializing keyboard...\n");
    init_keyboard();

    monitor_write("Initializing PC speaker and music player...\n");
    init_pcspkr();
    init_music_player();

    pcspkr_beep();
    monitor_write("\n=== HAMEDOS with Music Player ===\n");
    monitor_write("Press 'm' to enter music player mode\n");

    for (;;) {
        u8int scancode = inb(0x60);

        if (!(scancode & 0x80)) {
            char c = keyboard_scancode_to_char(scancode, keyboard_is_shift_pressed());

            if (c == 'm' && !music_player_active) {
                music_player_active = 1;
                monitor_write("\n=== Music Player Mode ===\n");
                monitor_write("Press 'h' for help or 'x' to exit\n");
            }
            else if (music_player_active) {
                handle_music_input(c);
            }
        }

        asm volatile("hlt");
    }
}
