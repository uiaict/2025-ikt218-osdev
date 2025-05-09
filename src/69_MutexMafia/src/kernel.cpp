extern "C"
{
#include "memory/malloc.h"
#include "memory/paging.h"
#include "utils/utils.h"
#include "idt/idt.h"
#include "io/keyboard.h"
#include "io/printf.h"
#include "pit/pit.h"
#include "music/songplayer.h"
#include "monitor/monitor.h"
#include "game/game.h"
}
extern "C" int kernel_main(void);

int kernel_main()
{
    init_monitor();
    //init_pit();
    init_highscores();
    clear_screen();
    print_mutexMafia();

    while (1)
    {
        print_menu();
        char input[50];
        get_input(input, sizeof(input));

        switch (input[0])
        {
        case '1':
            mafiaPrint("\nHello World!\n");
            break;
        case '2':
            mafiaPrint("\n");
            print_memory_layout();
            break;
        case '3':
        {
            int input_size = 0;
            mafiaPrint("\nEnter the size of memory to allocate: ");
            get_input(input, sizeof(input));
            input_size = stoi(input);
            void *address = malloc(input_size);
            break;
        }
        case '4':
            mafiaPrint("\nplay song\n");
            song_menu();
            break;
        case '5':
            mafiaPrint("\nPlaying @-Bird...\n");
            play_game();
            break;
        case '6':
            print_highscores();
            break;

        case '7':
            clear_screen();
            break;
        default:
            mafiaPrint("\nInvalid option. Please try again.\n");
        }
    }
}