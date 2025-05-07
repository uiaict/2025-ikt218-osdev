#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "libc/string.h"
#include "libc/gdt.h"
#include "libc/terminal.h"
#include "libc/printf.h"
#include "libc/idt.h"
#include "libc/irq.h"
#include "libc/keyboard.h"
#include "pit.h"
#include "memory.h"
#include "song.h"
#include "song_player.h"
#include "frequencies.h"
#include "matrix.h"


extern uint32_t end;

typedef enum {
    MODE_DEFAULT,
    MODE_MUSIC,
    MODE_MEMORY,
    MODE_TERMINAL
} InputMode;

__attribute__((noreturn)) void exception_handler(uint32_t int_number) {
    if (int_number >= 32 && int_number <= 47) {
        uint8_t irq = int_number - 32;

        if (irq == 0 && irq_handlers[irq]) {
            irq_handlers[irq]();
            irq_acknowledge(irq);
            return;
        }

        if (irq == 1) {
            keyboard_handler();
            irq_acknowledge(irq);
            return;
        }

        irq_acknowledge(irq);
        return;
    }

    printf("Exception: interrupt %d\n", (int)int_number);
    __asm__ volatile("cli; hlt");
    __builtin_unreachable();
}

SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    player->play_song = play_song_impl;
    return player;
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    init_gdt();
    idt_init();
    terminal_clear();
    //init_paging();                           
    init_kernel_memory(&end);
    init_pit();
    irq_install_handler(0, pit_callback);
    __asm__ volatile ("sti");


    disable_cursor();
    InputMode current_mode = MODE_DEFAULT;
    draw_front_page();

    while (1) {
        char key = get_last_key();
        if (key) {
            switch (current_mode) {
                case MODE_DEFAULT:
                    switch (key) {
                        case '1':
                            terminal_clear();
                            draw_matrix_rain();
                            break;
                        case '2':
                            current_mode = MODE_MUSIC;
                            terminal_clear();
                            draw_music_selection();
                            break;
                        case '3':
                            current_mode = MODE_MEMORY;
                            terminal_clear();
                            print_memory_layout();
                            printf("\n[esc] Back to main menu");
                            break;
                        case '4':
                            current_mode = MODE_TERMINAL;
                            terminal_clear();
                            break;
                        case 'q'|'Q':
                            terminal_clear();
                            printf("Shutting down!\n");
                            return 0;
                    }
                    break;

                case MODE_MUSIC:
                    switch (key) {
                        case '1':
                            terminal_clear();
                            printf("Playing Fur Elise...\n");
                            play_song(&furelise);
                            draw_music_selection();
                            break;
                        case '2':
                            terminal_clear();
                            printf("Playing Happy Birthday...\n");
                            play_song(&birthday);
                            draw_music_selection();
                            break;
                        case '3':
                            terminal_clear();
                            printf("Playing Star Wars Theme...\n");
                            play_song(&starwars);
                            draw_music_selection();
                            break;
                    }
                    break;

                case MODE_TERMINAL:
                    enable_cursor(14, 15);
                    printf("%c", key);
                    break;

                default:
                    break;
            }

            if (key == 27) { // ESC
                current_mode = MODE_DEFAULT;
                disable_cursor();
                terminal_clear();
                draw_front_page();
            }
        }

        __asm__ volatile("hlt");
    }
}
