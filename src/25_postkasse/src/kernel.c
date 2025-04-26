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

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    
    //Initialize gdt from gdt.h
    init_gdt();

    //Initialize idt from idt.h
    init_idt();

    //Initialize the keyboard
    init_keyboard();

    //Enable interrupts
    __asm__ volatile ("sti");

    //Interupt 0 test 
    //asm("int $0x0");

    init_kernel_memory(&end);

    init_paging();

    init_pit(); 

    while (1)
    {
        monitor_write("==== Knut OS ====\n");
        monitor_write("[1] Matrix Rain\n");
        monitor_write("[2] Play Music\n");
        monitor_write("[3] Print memory layout\n");
        monitor_write("[Q] Quit\n");
        monitor_write("==================\n");
        monitor_write("Press a key: ");
    
        char choice = keyboard_get_key();
    
        if (choice == '1') {
            clear_screen();
            run_matrix_rain();
        } else if (choice == '2') {
            monitor_write("\n==================\n");
            monitor_write("[1] Happy birthday\n");
            monitor_write("[2] Star Wars theme\n");
            monitor_write("[3] Fur Elise\n");
            monitor_write("Choose a song: \n");
            char choice = keyboard_get_key();
            clear_screen();

            if (choice == '1') {
                monitor_write("Playing Happy Birthday...\n");
                play_song(happy_birthday, HAPPY_BIRTHDAY_LENGTH);
            } else if (choice == '2') {
                monitor_write("Playing Star Wars theme...\n");
                play_song(star_wars_theme, STAR_WARS_THEME_LENGTH);
            } else if (choice == '3') {
                monitor_write("Playing Fur Elise...\n");
                play_song(fur_elise, FUR_ELISE_LENGTH);
            }
            
            else {
                monitor_write("Invalid choice. Halting...\n");
                while (1) { __asm__ volatile ("hlt"); }
            }

            {
                /* code */
            }
            

            
            clear_screen();
        } else if (choice == '3') {
            print_memory_layout();
        } else if (choice == 'q' || choice == 'Q') {
            return 0;
        }  else {
            monitor_write("Invalid choice. Halting...\n");
            while (1) { __asm__ volatile ("hlt"); }
        }
    }
    
    //infinite loop to keep the kernel running
    while (1) {
        __asm__ volatile ("hlt");
    }

    return 0;

}