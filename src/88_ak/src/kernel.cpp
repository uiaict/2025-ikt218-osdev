extern "C" {
#include "malloc.h"
#include "paging.h"
#include "utils.h"
#include "idt.h"
#include "keyboard.h"
#include "printf.h"
#include "pit.h"
#include "monitor.h"
#include "songplayer.h"
}
extern "C" int kernel_main(void);

int kernel_main() {
    init_monitor();
    clear_screen();
    asm volatile("sti");

    while (1) {
        print_menu();
        char input[50];
        get_input(input, sizeof(input));

        switch (input[0]) {
        case '1':
            Print("\nHello World!\n");
            break;
        case '2':
            Print("\n");
            print_memory_layout();
            break;
        case '3': {
            int input_size = 0;
            Print("\nEnter size of memory to allocate: ");
            get_input(input, sizeof(input));
            input_size = stoi(input);
            void *address = malloc(input_size);
            break;
        }
        case '4':
            Print("\nplay song\n");
            song_menu();
            break;
        case '5':
            Print("\nText editor\n");
            editor_mode();
            break;
        case '6':
            clear_screen();
            break;
        default:
            Print("\nInvalid option. Please try again.\n");       
        }
    }
}