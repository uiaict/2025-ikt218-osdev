#include "i386/keyboard.h"
#include "kernel/pit.h"
#include "kernel/memory.h"
#include "libc/stdio.h"
#include "matrix_effect/matrix.h"
#include "song/song.h"
#include "common.h"
#include "screen.h"
char key_buffer[255];
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

void wait_for_keypress()
{
    reset_key_buffer();
    while (key_buffer[0] == '\0')
    {
        // venter på at bruker trykker på en knapp
    }
}

char get_key()
{
    reset_key_buffer();
    while (key_buffer[0] == '\0')
    {
    }
    char key = key_buffer[0];

    return key;
}

void reset_key_buffer()
{
    for (int i = 0; i < 255; i++)
    {
        key_buffer[i] = '\0';
        if (key_buffer[i + 1] == '\0')
            break;
    }
}

void print_menu()
{
    clear_the_screen();
    printf("\n");
    printf("Welcome to the os for summernerds!\n");
    printf("\n");
    printf(" 1. Play Startup Song\n");
    printf(" 2. Matrix Rain Effect\n");
    printf(" 3. Play beep Sound\n");
    printf(" 4. Play pong (or some ganme)\n");
    printf(" 5. Turn of bye\n");
    printf("\n");
}

void handle_menu()
{
    while (1)
    {
        print_menu();
        char choice = get_key();
        printf("%d", choice);
        printf("\n");

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
            init_matrix();
            while (1)
            {
                draw_matrix_frame();
                sleep_interrupt(100);
                if (key_buffer[0] != '\0')
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
            // call runthegame function when finished to be made
        }

        case '5':
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
