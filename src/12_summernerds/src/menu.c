#include "screen.h"
#include "song/song.h"
#include "matrix.h"
#include "common.h"
#include "keyboard.h"

void shutdown()
{

    outw(0x604, 0x2000);
    outb(0xB004, 0x2000);

    asm volatile("cli");
    while (1)
    {
        asm volatile("hlt");
    }
}

void wait_for_keypress()
{
    while (key_buffer[0] == '\0')
    {
        // venter på at bruker trykker pa en knapp
    }

    for (int i = 0; i < 255; i++)
    {
        key_buffer[i] = key_buffer[i + 1];
        if (key_buffer[i + 1] == '\0')
            break;
    }
}

char get_key()
{
    while (key_buffer[0] == '\0')
    {
    }
    char key = key_buffer[0];

    for (int i = 0; i < 255; i++)
    {
        key_buffer[i] = key_buffer[i + 1];
        if (key_buffer[i + 1] == '\0')
            break;
    }

    return key;
}

void print_menu()
{
    clear_screen();
    print("\n");
    print("Welcome to the os for summernerds!\n");
    print("\n");
    print(" 1. Play Startup Song\n");
    print(" 2. Matrix Rain Effect\n");
    print(" 3. Play beep Sound\n");
    print(" 4. Play pong (or some ganme)\n");
    print(" 5. Turn of bye\n");
    print("\n");
}

void handle_menu()
{
    while (1)
    {
        print_menu();
        char choice = get_key();
        print_char(choice);
        print("\n");

        switch (choice)
        {
        case '1':
        {
            print("Playing startup song...\n");
            Song song = {music_1, sizeof(music_1) / sizeof(Note)};
            play_song(&song);
            break;
        }

        case '2':
        {
            print("Starting Matrix Rain effect...\n");
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
            print("Beep!\n");
            beep();
            break;
        }

        case '4':
        {
            // call runthegame function when finished to be made
        }

        case '5':
        {
            print("Shutting down...\n");
            shutdown();
            return;
        }

        default:
        {
            print("Option not acceptable. Please try again...\n");
            break;
        }
        }

        print("\nPress any key in order to return to get back to menu...");
        wait_for_keypress();
    }
}
