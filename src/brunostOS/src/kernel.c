#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "libc/stdio.h"
#include <multiboot2.h>
#include "descriptor_tables.h"
#include "keyboard.h"
#include "timer.h"
#include "speaker.h"
#include "song/song.h"
#include "io.h"
#include "libc/stdlib.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "paint.h"
#include "demo.h"

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info *mb_info_addr) {
    
    set_vga_color(VGA_WHITE, VGA_BLACK);
    init_gdt();
    init_idt();
    init_keyboard();
    uint32_t begin = init_kernel_memory(&end);
    init_paging();
    uint32_t pit_Hz = 500;
    init_pit(500);

    printf("initializing gdt...\n\r");
    busy_sleep(100);
    printf("initializing idt...\n\r");
    busy_sleep(100);
    printf("initializing memory...\n\r");
    busy_sleep(300);
    printf("kernel heap starts at 0x%x\n\r", begin);
    printf("enablling paging...\n\r");
    busy_sleep(100);
    printf("initializing keyboard with mapping:no...\n\r");
    busy_sleep(100);
    printf("initializing pit at %uHz...\n\r", pit_Hz);
    busy_sleep(100);
    printf("enabling speakers...\n\r");
    busy_sleep(400);

    set_freewrite(true);


    
    enum vga_color txt_clr = get_vga_txt_clr();
    enum vga_color bg_clr = get_vga_bg_clr();
    char *test_storage = malloc(STORAGE_SPACE);
    while (true){
        clear_terminal();
        reset_cursor_pos();
        print_main_menu();
        int c = getchar();
        clear_terminal();
        reset_cursor_pos();
        update_cursor();

        switch (c){
            case '1':
                change_terminal_color();
                txt_clr = get_vga_txt_clr();
                bg_clr = get_vga_bg_clr();
                break;
            case '2':
                set_freewrite(false);
                freewrite();
                set_freewrite(true);
                break;
            case '3':
                print_memory_layout();
                getchar(); // press any key to continue;
                break;
            case '4':
                music_player();
                break;
            case '5':
                txt_clr = get_vga_txt_clr();
                bg_clr = get_vga_bg_clr();
                paint(painting1, test_storage);
                break;
            case '6':
                suicide();
                break;
            
            default:
                break;
        }
        set_vga_color(txt_clr, bg_clr);
        
    }
    free(test_storage);
  
    
//TODO: 
//io buffer overflow
//memory
    return 0;
}