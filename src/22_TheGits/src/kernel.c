#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "libc/stdarg.h"
#include "libc/gdt.h" 
#include "libc/scrn.h"
#include "libc/idt.h"
#include "libc/isr_handlers.h"
#include "libc/irq.h"
#include "memory/memory.h"
#include "pit/pit.h"
#include "audio/song.h"
#include "audio/player.h"
#include "audio/tracks.h"
#include "game/wordgame.h"
#include "menu.h"
#include "system.h"

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

extern uint32_t end; // End of kernel memory

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

   
    // === SYSTEMINIT ===
    init_gdt();
    remap_pic();
    init_idt();
    init_irq();
    init_kernel_memory(&end);
    init_paging();
    init_pit();

    __asm__ volatile ("sti"); // Activate interrupts

    // === SCREEN STARTUP ===

    print_os_greeting();
    char choice[5];    

  while(choice[0] != '5') {
    printf("MENU:\n");
    printf("1: Play word game\n");
    printf("2: Play music\n");
    printf("3: Memory management menu\n");
    printf("4: Check PIT functions\n");
    printf("5: Shut down\n");
    printf("Please choose an option (1-5): ");

    // === MENU CHOICES===

    get_input(choice, sizeof(choice));

    if (choice[0] == '1') {
        start_game_menu();
    } else if (choice[0] == '2') {
        play_music_menu();
    } else if (choice[0] == '3') {
        memory_menu();
    } else if (choice[0] == '4') {
        pit_menu();
    } else if (choice[0] == '5') {
        print_os_farewell();
        sleep_busy(3000);
        shutdown();
    }
    else {
        printf("Invalid input, please try again..\n");
    }
}

    // === REST ===
   while (1) {
        __asm__ volatile ("hlt");
    }

    return 0;
}
