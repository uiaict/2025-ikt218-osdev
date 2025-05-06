#include "i386/keyboard.h"
#include "kernel/pit.h"
#include "kernel/memory.h"
#include "libc/stdio.h"
#include "matrix_effect/matrix.h"
#include "song/song.h"
#include "common.h"
#include "menu.h"
#include "game/game.h"
#include "i386/monitor.h"

void reset_key_buffer();

void shutdown()
{

    outb(0x604, 0x20);
    outb(0xB004, 0x20);

    asm volatile("cli");
    while (1)
    {
        asm volatile("hlt");
    }
}

void print_menu()
{
    monitor_clear();
    monitor_setcolor(13);
    printf("\\\\                            //\n"
           " \\\\                          // \n"
           "  \\\\                        //  \n"
           "   \\\\                      //   \n"
           "    \\\\        //\\\\        //    \n" // for wanted result, it looks weird here in editor
           "     \\\\      //  \\\\      //     \n"
           "      \\\\    //    \\\\    //      \n"
           "       \\\\  //      \\\\  //       \n"
           "        \\\\//        \\\\//        \n");
    monitor_setcolor(12);
    printf("Welcome to the os for summernerds!\n"
           "\n"
           " 1. Play Startup Song\n"
           " 2. Matrix Rain Effect\n"
           " 3. Play beep Sound\n"
           " 4. Write text (similar to notepad)\n"
           " 5. Play pong.\n"
           " 6. Exit\n"
           "\n");
}

void handle_menu()
{
    EnableBufferTyping();
    while (1)
    {
        print_menu();
        char choice = get_key();
        putchar(choice);
        putchar('\n');

        switch (choice)
        {
        case '1':
        {
            printf("Playing startup song...\n");
            Song song = {music_1, sizeof(music_1) / sizeof(Note)};
            play_song(&song);
            break;
        }

        case '2':
        {
            printf("Starting Matrix Rain effect...\n");
            reset_key_buffer();
            init_matrix();
            while (1)
            {
                draw_matrix_frame();
                sleep_interrupt(80);
                if (get_first_buffer())
                    break;
            }
            break;
        }

        case '3':
        {
            printf("Beep!\n");
            beep();
            break;
        }

        case '4':
        {
            DisableBufferTyping();
            monitor_clear();
            EnableTyping();
            while (true)
            {
                if (has_user_pressed_esc())
                {
                    DisableTyping();
                    break;
                }
            }
            EnableBufferTyping();
            break;
        }
        case '5':
            run_pong();
            break;
        case '6':
        {
            printf("Shutting down...\n");
            shutdown();
            return;
        }

        default:
        {
            printf("Option not acceptable. Please try again...\n");
            break;
        }
        }

        printf("\nPress any key in order to return to get back to menu...");
        wait_for_keypress();
    }
}
