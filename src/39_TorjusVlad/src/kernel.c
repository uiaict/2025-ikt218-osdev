#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <libc/stdio.h>
#include "libc/portio.h"
#include <libc/memory.h>
#include "arch/i386/fpu.h"
#include "arch/i386/console.h"
#include <multiboot2.h>
#include "gdt.h"
#include "arch/i386/idt.h"
#include "keyboard.h"
#include "libc/paging.h"
#include "pmalloc.h"
#include "pit.h"
#include "song/music_1.h"
#include "song_player.h"
#include "libc/random.h"
#include "games/snake.h"

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

static inline void shutdown_qemu() {
    outw(0x604, 0x2000);
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    printf("Hello, World! Magic number: %d\n", magic);

    init_gdt();
    printf("GDT was initialized successfully\n");

    init_fpu();
    printf("FPU was initialized successfully\n");
    printf("Test fpu 2.0 * 2.5f = %f\n", (2.0f * 2.5f));

    init_idt();
    printf("IDT was initialized successfully\n");

    enable_interrupts();
    printf("Enabling interrupts\n");

    // Trigger interrupt manually to test
    //__asm__ volatile ("int $0");
    //printf("%d", 1/0);
    //__asm__ volatile ("int $1");
    //__asm__ volatile ("int $2");

    init_keyboard();
    printf("The keeyboard was initialized successfully\n");
    

    printf("Setting up Paging... \n");
    init_paging();
    enable_paging();
    printf("Paging enabled! \n");
    
    init_kernel_memory(&end);
    printf("Initialized kernel memory\n");
    
    printf("Testing malloc...\n");
    char* heap_test = (char*)malloc(128);
    printf("malloc returned: 0x%x\n", (uint32_t)heap_test);
    print_memory_layout();

    init_pit();
    printf("PIT was initialized succesfully\n");
    
    int counter = 0;
    printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
    sleep_busy(1000);
    printf("[%d]: Slept using busy-waiting.\n", counter++);

    printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
    sleep_interrupt(1000);
    printf("[%d]: Slept using interrupts.\n", counter++);   
   
    while (1) {
        console_clear();
        printf("Welcome to the UIA OS!\n\n");
        printf("Select an option:\n");
        printf("  [1] Play Music\n");
        printf("  [2] Play Snake\n");
        printf("  [Q] Shutting down\n");
    
        while (1) {
            char choice = keyboard_get_char();
            if (choice == '1') {
                console_clear();
                play_music();
                break;
            } else if (choice == '2') {
                console_clear();
                snake_main();
                break;
            } else if (choice == 'q' || choice == 'Q') {
                console_clear();
                printf("Shutting down...\n");
                shutdown_qemu();
                return 0;
            }
        }
    }

    return 0;

}