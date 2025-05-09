#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"

void clear_screen(void);

#include "kernel_memory.h"
#include "pit.h"
#include "paging.h"
#include "interrupt.h"
#include "io.h"
#include "song.h"
#include "keyboard.h"
#include "menu.h"
#include "matrix.h"
#include "interrupt_test.h"
#include "music_player.h"

void draw_outline(void)
{
    puts("+----------------------------------------+\n");
}

static void print_main_menu(void)
{
    clear_screen();
    set_color(0x0B);

    draw_outline();
    puts("|               Main Menu               |\n");
    draw_outline();
    puts("|  [1] Memory & Interrupt Test          |\n");
    draw_outline();
    puts("|  [2] Music player                     |\n");
    draw_outline();
    puts("|  [3] Matrix rain                      |\n");
    draw_outline();
}

int kernel_main()
{
    sleep_interrupt(4000);

    KernelMode mode = MODE_NONE;
    char last_key = 0;

    print_main_menu();

    while (true)
    {
        char current_key = keyboard_get_last_char();
        if (last_key != current_key && current_key != 0)
        {
            last_key = current_key;
            puts("Key pressed: ");
            putchar(current_key);
            puts("\n");

            if (mode == MODE_NONE)
            {
                switch (last_key)
                {
                case '1':
                    clear_screen();
                    mode = MODE_TEST;
                    set_color(0x0B);
                    puts("Press any key to test\n");
                    break;
                case '2':
                    mode = MODE_MUSIC_MENU;
                    music_player_show_menu();
                    keyboard_clear_last_char();
                    last_key = 0;
                    break;
                case '3':
                    clear_screen();
                    set_color(0x0B);
                    matrix();
                    mode = MODE_NONE;
                    keyboard_clear_last_char();
                    last_key = 0;
                    print_main_menu();
                    break;
                }
                keyboard_clear_last_char();
            }
            else if (mode == MODE_MUSIC_MENU)
            {
                mode = music_player_handle_input(last_key);
                keyboard_clear_last_char();
            }
            else if (mode == MODE_TEST)
            {
                run_memory_interrupt_test();
                mode = MODE_NONE;
                keyboard_clear_last_char();
                print_main_menu();
            }
        }

        if (mode == MODE_MUSIC_PLAYER)
        {
            KernelMode next_mode = music_player_update();
            if (next_mode != MODE_MUSIC_PLAYER)
            {
                mode = next_mode;
                if (mode == MODE_NONE)
                {
                    print_main_menu();
                }
            }
        }

        if (mode == MODE_NONE)
        {
            music_player_cleanup();
            last_key = 0;
        }
    }

    music_player_cleanup();
    return 0;
}
