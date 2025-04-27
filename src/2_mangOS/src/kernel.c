#include "libc/stdbool.h"
#include "multiboot2.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/string.h"
#include "libc/stdio.h"
#include "libc/terminal.h"
#include "memory/memory.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "pit.h"
#include "music/songplayer.h"
#include "view.h"
#include "dev/cli.h"

struct multiboot_info
{
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

extern uint32_t end;

int main(uint32_t magic, struct multiboot_info *mb_info_addr)
{
    terminal_initialize();
    printf("Terminal initialized\n");
    init_gdt();
    printf("GDT initialized\n");
    // Initialize the Global Descriptor Table (GDT).
    init_gdt();
    printf("GDT initialized\n");

    // Initialize the Interrupt Descriptor Table (IDT).
    init_idt();
    printf("IDT initialized\n");

    // Initialize the hardware interrupts.
    init_irq();
    printf("IDT initialized\n");

    init_isr_handlers();
    init_irq_handlers();
    printf("ISR handlers initialized\n");

    init_keyboard();
    printf("Keyboard initialized\n");

    init_kernel_memory((uint32_t *)&end);
    init_paging();
    printf("Kernel memory initialized & paging\n");

    // print_memory_layout();
    init_pit();

    asm volatile("sti");
    printf("Interrupts enabled\n");

    void *ptr = malloc(12345);
    void *ptr2 = malloc(54321);
    void *ptr3 = malloc(100);

    // Testing interrupt 3, 4 & 5
    printf("Testing interrupts...\n");
    asm volatile("int $0x3");
    asm volatile("int $0x4");
    asm volatile("int $0x5");
    printf("Sleeping for 5 seconds...\n");
    sleep_interrupt(5000);
    // Uncomment to cause panic
    // asm volatile("int $0x6");

    SongPlayer *player = create_song_player();

    Song victorySong = {victory, sizeof(victory) / sizeof(Note), "Victory Theme"};
    Song starWars = {starwars_theme, sizeof(starwars_theme) / sizeof(Note), "Star Wars Theme"};

    // for (uint32_t i = 0; i < n_songs; i++)
    // {
    //     printf("Playing song...\n");
    //     player->play_song(songs[i]);
    //     printf("Done!\n");
    // }
    int choice;
    do
    {
        choice = menu();
        switch (choice)
        {
        case 1:
            printf("Playing song...\n");

            player->play_song(&victorySong);
            break;

        case 2:
            print_memory_layout();
            printf("Press any key to continue...\n");
            getChar();
            break;

        case 3:
            start_cli();
            break;

        default:
            break;
        }
    } while (choice != 4);

    printf("Exiting...\n");

    return 0;
}