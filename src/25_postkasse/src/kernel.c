#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "libc/monitor.h"
#include "libc/memory/memory.h"
#include "libc/pit.h"
#include "libc/music.h"
#include <multiboot2.h>
#include "arch/i386/gdt/gdt.h"
#include "arch/i386/idt/idt.h"
#include "libc/frequencies.h"
#include "libc/song.h"
#include "matrix/matrix_rain.h"

extern uint32_t end;
extern int cursor_x;
extern int cursor_y;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

void move_cursor(int x, int y) {
    cursor_x = x;
    cursor_y = y;
}

void centerTextAtLine(const char *string, int y){
    int length = strlen(string);
    int offset = VGA_WIDTH / 2 - length / 2;
    move_cursor(offset, y);
    monitor_write(string);
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    
    init_gdt();
    init_idt();
    init_keyboard();
    __asm__ volatile ("sti");
    //asm("int $0x0");
    init_kernel_memory(&end);
    init_paging();
    init_pit(); 

    while (1) {
        clear_screen();
        centerTextAtLine("==== Postkasse OS ====", 5);
        centerTextAtLine("[1] Matrix Rain", 7);
        centerTextAtLine("[2] Play Music", 9);
        centerTextAtLine("[3] Print Memory Layout", 11);
        centerTextAtLine("[Q] Quit", 13);

        char choice = keyboard_get_key();
    
        if (choice == '1') {
            clear_screen();
            run_matrix_rain();
            
        } else if (choice == '2') {
            clear_screen();
            centerTextAtLine("[1] Happy birthday", 5);
            centerTextAtLine("[2] Star Wars theme", 7);
            centerTextAtLine("[3] Fur Elise", 9);
            centerTextAtLine("[Q] Quit", 11);
            char choice = keyboard_get_key();

            clear_screen();

            if (choice == '1') {
                monitor_write(" Happy Birthday...\n");
                play_song(happy_birthday, HAPPY_BIRTHDAY_LENGTH);
            } else if (choice == '2') {
                monitor_write("Playing Star Wars theme...\n");
                play_song(star_wars_theme, STAR_WARS_THEME_LENGTH);
            } else if (choice == '3') {
                monitor_write("Playing Fur Elise...\n");
                play_song(fur_elise, FUR_ELISE_LENGTH);
            } else if (choice == 'q' || choice == 'Q') {
                monitor_write("Exiting music...\n");
            } else {
                monitor_write("Invalid choice.\n");
            }
            clear_screen();

        } else if (choice == '3') {
            clear_screen();
            print_memory_layout();
            char choice = keyboard_get_key();
        } else if (choice == 'q' || choice == 'Q') {
            return 0;
        }  else {
            centerTextAtLine("Invalid choice", 15);
        }
    }
    
    

    return 0;

}