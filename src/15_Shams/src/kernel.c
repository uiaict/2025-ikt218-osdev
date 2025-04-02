#include "libc/stdbool.h"
#include <multiboot2.h>
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/memory.h"
#include "song/song.h"
#include "song/music.h"
#include "libc/song_player.h"

// Pointer to video memory at address 0xB8000 (used for text mode display)
volatile uint16_t *video_memory = (uint16_t *)0xB8000;
// Cursor position (keeps track of where the next character will be printed)
int cursor_x = 0, cursor_y = 0;

extern Song song1, song2, song3, song4, song5, song6;
extern void play_song_impl(Song *song);

// Write a single character to the screen
void terminal_putc(char c)
{
    // If the character is a newline, move to the next line
    if (c == '\n')
    {
        cursor_x = 0; // Reset cursor to the start of the line
        cursor_y++;   // Move to the next row
    }
    else
    {
        // Store the character in video memory at the current cursor position
        // (0x0F << 8) sets the text color (white on black)
        video_memory[cursor_y * 80 + cursor_x] = (0x0F << 8) | c;

        // Move the cursor to the right
        cursor_x++;
    }
}

// Write a string to the screen
void terminal_write(const char *str)
{
    // Loop through each character in the string until we reach the end ('\0')
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        // Write the current character to the screen using the terminal_putc function
        terminal_putc(str[i]);
    }
}

// Function to check if the GDT is loaded correctly
void check_gdt()
{
    uint16_t cs, ds; // Variables to store the values of the code segment (cs) and data segment (ds) registers

    // Assembly instruction to move the value of the Code Segment (CS) register into the cs variable
    __asm__ __volatile__("mov %%cs, %0" : "=r"(cs));

    // Assembly instruction to move the value of the Data Segment (DS) register into the ds variable
    __asm__ __volatile__("mov %%ds, %0" : "=r"(ds));

    // Print a message to the screen checking the GDT
    terminal_write("\nChecking GDT:\n");

    // Check if the GDT is loaded correctly by comparing the values of the CS and DS registers
    if (cs == 0x08 && ds == 0x10)
    {
        terminal_write("GDT Loaded Successfully\n");
    }
    else
    {
        terminal_write("GDT Failed\n");
    }
}

int main(uint32_t magic, struct multiboot_info *mb_info_addr)
{

    // Print "Hello World" to the screen
    terminal_write("Hello World\n");
    init_gdt();    // Initialize the Global Descriptor Table (GDT)
    check_gdt();   // Check if the GDT is loaded correctly
    init_idt();    // Initialize IDT
    isr_install(); // Install ISRs
    init_irq();    // Initialize IRQs
    terminal_write("Interrupts are set up!\n");
    init_pit();
    __asm__ __volatile__("sti"); // Aktiver maskinvare-interrupts

    extern uint32_t end;
    init_kernel_memory(&end);
    print_memory_layout();

    terminal_write("Press 1–6 to play a song.\n");

    terminal_write("Playing all songs...\n");
    play_song_impl(&song1);
    play_song_impl(&song2);
    play_song_impl(&song3);
    play_song_impl(&song4);
    play_song_impl(&song5);
    play_song_impl(&song6);

    int counter = 0;
    while (1)
    {
        terminal_write("Busy wait sleep...\n");
        sleep_busy(1000);

        terminal_write("Interrupt sleep...\n");
        sleep_interrupt(1000);

        counter++;
    }

    return 0;
}
