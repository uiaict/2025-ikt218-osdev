#include "libc/stdbool.h"
#include <multiboot2.h>
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/memory.h"
#include "song/song.h"
#include "song/music.h"
#include "libc/song_player.h"
#include "libc/terminal.h" // <-- Viktig: Bruk terminal_putc/terminal_write fra terminal.c
#include "libc/idt.h"
#include "libc/isr.h"
#include "libc/irq.h"
#include "libc/keyboard.h"
#include "libc/pit.h"

// Disse er allerede definert i terminal.c, så IKKE definer dem her!
// volatile uint16_t *video_memory;
// int cursor_x, cursor_y;

extern Song song1, song2, song3, song4, song5, song6;
extern void play_song_impl(Song *song);

// Sjekker om GDT er riktig lastet
void check_gdt()
{
    uint16_t cs, ds;

    __asm__ __volatile__("mov %%cs, %0" : "=r"(cs));
    __asm__ __volatile__("mov %%ds, %0" : "=r"(ds));

    terminal_write("\nChecking GDT:\n");

    if (cs == 0x08 && ds == 0x10)
    {
        terminal_write("GDT Loaded Successfully\n");
    }
    else
    {
        terminal_write("GDT Failed\n");
    }
}

// Main entry point
int main(uint32_t magic, struct multiboot_info *mb_info_addr)
{
    terminal_write("Hello World\n");

    init_gdt();    // Initialize the Global Descriptor Table (GDT)
    check_gdt();   // Check if GDT is correct
    init_idt();    // Initialize IDT
    isr_install(); // Install ISRs
    init_irq();    // Initialize IRQs
    terminal_write("Interrupts are set up!\n");
    init_pit();      // Initialize timer (PIT)
    keyboard_init(); // Initialize keyboard

    __asm__ __volatile__("sti"); // Enable interrupts globally

    extern uint32_t end;
    init_kernel_memory(&end); // Setup memory manager
    print_memory_layout();    // Print memory info

    terminal_write("Playing all songs...\n");

    __asm__ __volatile__("cli"); // DISABLE interrupts
    play_song_impl(&song1);
    play_song_impl(&song2);
    play_song_impl(&song3);
    play_song_impl(&song4);
    play_song_impl(&song5);
    play_song_impl(&song6);

    __asm__ __volatile__("sti"); // ENABLE interrupts

    start_matrix_rain(); // <--- START ANIMASJONEN

    int counter = 0;
    /*
    while (1)
    {
        terminal_write("Busy wait sleep...\n");
        sleep_busy(1000);

        terminal_write("Interrupt sleep...\n");
        sleep_interrupt(1000);

        counter++;
    }
    */

    while (1)
    {
    }
}
