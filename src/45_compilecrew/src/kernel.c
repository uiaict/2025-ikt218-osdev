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
#include "memory.h"   // include memory manager
#include "song.h"
#include "song_player.h"
#include "frequencies.h"

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

__attribute__((noreturn)) void exception_handler(uint32_t int_number) {
    if (int_number >= 32 && int_number <= 47) {
        uint8_t irq = int_number - 32;

        if (irq == 0 && irq_handlers[irq]) {
            irq_handlers[irq]();
            irq_acknowledge(irq);
            return; // ✅ just return, NO halt
        }

        if (irq == 1) {
            keyboard_handler();
            irq_acknowledge(irq);
            return; // ✅ just return, NO halt
        }

        irq_acknowledge(irq);
        return; // ✅ normal IRQ return
    }

    // 🛑 Only halt for real EXCEPTIONS (divide by zero, etc)
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




    init_kernel_memory(&end);
    //init_paging();
    print_memory_layout();

    init_pit();
    irq_install_handler(0, pit_callback);
    __asm__ volatile ("sti");

    printf("Hello World\n");

    terminal_clear();

    draw_front_page();
    disable_cursor();


    //void* some_memory = malloc(12345);
    //void* memory2 = malloc(54321);
    //void* memory3 = malloc(13331);

    
    Song song1 = { fur_elise, sizeof(fur_elise) / sizeof(Note) };
    Song song2 = { happy_birthday, sizeof(happy_birthday) / sizeof(Note) };
    Song song3 = { starwars_theme, sizeof(starwars_theme) / sizeof(Note) };
    



    while (1) {
        __asm__ volatile("hlt");
    }
}

