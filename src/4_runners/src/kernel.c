#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "libc/stdio.h"
#include "terminal.h"
#include "idt.h"
#include "gdt.h"
#include "irq.h"
#include "pit.h"
#include "io.h"
#include "memory.h"
#include "snake.h"
#include "song.h"
#include <multiboot2.h>

#define COLOR_TITLE 0x0E      // Yellow
#define COLOR_SNAKE_BODY 0x0A // Green
#define COLOR_SCORE 0x0B      // Cyan
#define COLOR_FOOD 0x0C


/* ==========================================================================
   External Symbols and Declarations
   ========================================================================== */
extern char end;

/* ==========================================================================
   Function Declarations
   ========================================================================== */
void terminal_write(const char *str);
void terminal_put_char(char c);
void initkeyboard(void);

/* ==========================================================================
   Multiboot Structure
   ========================================================================== */
struct multiboot_info
{
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

/* ==========================================================================
   Helper Functions
   ========================================================================== */
/**
 * Prints a 32-bit value in hexadecimal format.
 */
void print_hex(uint32_t value)
{
    const char *hex_digits = "0123456789ABCDEF";
    char buffer[11] = "0x00000000";
    for (int i = 9; i >= 2; i--)
    {
        buffer[i] = hex_digits[value & 0xF];
        value >>= 4;
    }
    terminal_write(buffer);
}

/**
 * Simple addition function for testing.
 */
int compute(int a, int b)
{
    return a + b;
}

/**
 * Sample struct for testing multiboot cast.
 */
typedef struct
{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e[6];
} myStruct;

/**
 * Verifies the state of PIT Channel 2.
 */
void verify_pit_channel2()
{
    // Read back command
    outb(PIT_CMD_PORT, 0xE8); // Read-back command for channel 2

    // Read status
    uint8_t status = inb(PIT_CHANNEL2_PORT);
    printf("PIT Channel 2 Status: 0x%02x\n", status);

    // Read current count
    uint8_t count_low = inb(PIT_CHANNEL2_PORT);
    uint8_t count_high = inb(PIT_CHANNEL2_PORT);
    uint16_t count = (count_high << 8) | count_low;

    printf("PIT Channel 2 Count: %u\n", count);

    // Reset Channel 2 to known state
    outb(PIT_CMD_PORT, 0xB6); // Channel 2, square wave mode
}

/**
 * Simple busy-wait delay for approximately 'seconds' seconds.
 */
void delay(int seconds)
{
    // Increased delay to approximately 5 seconds per step
    for (int i = 0; i < seconds * 5000000; i++)
    {
        asm volatile("nop"); // No-op instruction to consume CPU cycles
    }
}

/* ==========================================================================
   Kernel Main Loop
   ========================================================================== */
int kernel_main(void)
{
    /* ----------------------------------------------------------------------
       Initialize Core Components with Visible Boot Messages
       ---------------------------------------------------------------------- */
    terminal_clear();
    terminal_write("System initializing...\n");
    delay(5);

    terminal_write("Initializing Global Descriptor Table (GDT)...\n");
    gdt_init();
    delay(5);

    terminal_write("Initializing Interrupt Descriptor Table (IDT)...\n");
    idt_init();
    delay(5);

    terminal_write("Initializing hardware interrupts (IRQ)...\n");
    irq_init();
    delay(5);

    // Initial "Hello, World!" test
    terminal_write("Hello, World!\n");

    // Interrupt testing
    asm volatile("int $0");
    asm volatile("int $1");
    asm volatile("int $2");

    terminal_write("Initializing Programmable Interval Timer (PIT)...\n");
    init_pit();
    verify_pit_channel2();
    delay(5);

    // Add after PIT initialization
    terminal_write("Initializing PC Speaker...\n");
    // Reset Channel 2 to known state
    // outb(PIT_CMD_PORT, 0xB6);   // Channel 2, square wave mode
    // outb(PIT_CHANNEL2_PORT, 0); // Low byte
    // outb(PIT_CHANNEL2_PORT, 0); // High byte

    delay(5);

    terminal_write("Initializing memory...\n");
    init_kernel_memory((uint32_t *)&end);
    init_paging();
    delay(5);

    terminal_write("Initializing keyboard...\n");
    initkeyboard();
    delay(5);
    speaker_control(true);
    // Sound player testing
    static Note test_notes[] = {
        {.frequency = NOTE_C4, .duration = 1000},  // C4
        {.frequency = NOTE_E4, .duration = 1000},  // E4
        {.frequency = NOTE_G4, .duration = 1000},  // G4
        {.frequency = NOTE_C5, .duration = 1000}   // C5
    };

    Song test_song = {
        .notes = test_notes,
        .length = sizeof(test_notes) / sizeof(Note)
    };
    printf("\nTesting PC Speaker...\n");
    SongPlayer *player = create_song_player();
    if (player)
    {
        printf("Playing test notes...\n");
        player->play_song(&test_song);
        printf("Test complete\n");
    }
    else
    {
        printf("Failed to create song player.\n");
    }
    delay(5);

    terminal_write("System initialized successfully!\n");
    delay(5);
   speaker_control(false); // Turn off speaker

    // Enable interrupts globally
    asm volatile("sti");

menu_start:
    /* ----------------------------------------------------------------------
       Display Menu
       ---------------------------------------------------------------------- */
    terminal_clear();
    printf("Welcome to 4_runners!\n");
    printf("================\n\n");
    printf("Available Options:\n");
    printf("1. Snake Game\n");
    printf("2. Memory Layout\n\n");
    printf("Press 1-2 to select option...\n");

    /* ----------------------------------------------------------------------
       Main Loop
       ---------------------------------------------------------------------- */
    while (1)
    {
        char key = keyboard_getchar();
        if (key == '1')
        {
            terminal_clear();
            terminal_set_color(COLOR_TITLE);
            printf("\n");
            printf("  ____       _    _         _          _   __     _____ \n");
            printf(" / ___|     | \\ | |       / \\       | | / /    | ____|\n");
            printf(" \\_ \\     |  \\| |      / _ \\      |  | /     |  _|  \n");
            printf("  ___) |    | |\\  |     / ___ \\     | . \\     | |___ \n");
            printf(" |____/     |_| \\_|    /_/   \\_\\   |_|  \\_    |_____|\n");
            printf("\n");
            terminal_set_color(COLOR_SNAKE_BODY);
            printf("          /^\\/^\\                                                      \n");
            printf("        _|__|  O|                                                     \n");
            printf(" \\/     /~     \\_/ \\                                                 \n");
            printf("  \\____|__________/  \\                                                \n");
            printf("         \\_______      \\                                              \n");
            printf("                 `\\     \\                 \\                           \n");
            printf("                   |     |                  \\                         \n");
            printf("                  /      /                    \\                       \n");
            printf("                 /     /                       \\\\                    \n");
            printf("               /      /                         \\ \\                  \n");
            printf("              /     /                            \\  \\                \n");
            printf("            /     /             _----_            \\   \\              \n");
            printf("           /     /           _-~      ~-_         |   |              \n");
            printf("          (      (        _-~    _--_    ~-_     _/   |              \n");
            printf("           \\      ~-____-~    _-~    ~-_    ~-_-~    /               \n");
            printf("             ~-_           _-~          ~-_       _-~                 \n");
            printf("                ~--______-~                ~-___-~                    \n");

            terminal_set_color(COLOR_FOOD);
            printf("  Press any key to start!\n");
            terminal_set_color(0x07); // Reset color

            while (keyboard_getchar() == 0)
            {
                asm volatile("hlt");
            }

            SnakeGame *snake = create_snake_game();
            if (snake)
            {
                terminal_clear();
                snake->init();
                set_game_mode(true);

                bool running = true;
                while (running)
                {
                    if (!get_game_mode())
                    {
                        // Game mode was disabled (e.g., by pressing ESC in snake.c)
                        running = false;
                        goto menu_start; // Return to the menu
                    }

                    uint8_t input = keyboard_getchar();
                    if (input != 0)
                    {
                        snake->handle_input(input);
                    }
                    asm volatile("hlt");
                }
            }
        }
        else if (key == '2')
        {
            terminal_clear();
            printf("Current Memory Layout:\n");
            printf("=====================\n\n");
            print_memory_layout();
            printf("\nPress any key to return to menu...\n");

            while (keyboard_getchar() == 0)
            {
                asm volatile("hlt");
            }
            goto menu_start;
        }
        asm volatile("hlt");
    }

    return 0;
}

/* ==========================================================================
   Boot Entry
   ========================================================================== */
int main(uint32_t structAddr, uint32_t magic, struct multiboot_info *mb_info_addr)
{
    // Kernel end
    terminal_write("Kernel end = ");
    print_hex((uint32_t)&end);
    terminal_write("\n");

    // Testing multiboot struct
    myStruct *myStructPtr = (myStruct *)structAddr;
    int result = compute(1, 2);

    // Test malloc
    terminal_write("\nAllocating 64 bytes...\n");
    void *ptr = malloc(64);
    if (ptr)
    {
        terminal_write("Memory allocated.\n");
    }
    else
    {
        terminal_write("Memory allocation failed!\n");
    }

    terminal_write("\nMemory Layout After Allocation:\n");
    print_memory_layout();

    terminal_write("\nFreeing memory...\n");
    free(ptr);
    terminal_write("Memory freed.\n");

    terminal_write("\nMemory Layout After Deallocation:\n");
    print_memory_layout();

    return kernel_main();
}

/* ==========================================================================
   Commented-Out Experimental Code (For Submission Context)
   ========================================================================== */
/*
 * Initial "Hello, World!" test
 * terminal_write("Hello, World!\n");
 *
 * Interrupt testing
 * asm volatile("int $0");
 * asm volatile("int $1");
 * asm volatile("int $2");
 *
 * Sleep testing with busy-waiting and interrupts
 * int counter = 0;
 * while (1) {
 *     printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
 *     sleep_busy(1000);
 *     printf("[%d]: Slept using busy-waiting.\n", counter++);
 *
 *     printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
 *     sleep_interrupt(1000);
 *     printf("[%d]: Slept using interrupts.\n", counter++);
 * }
 *
 * Sound player testing
 * static Note test_notes[] = {
 *     {440, 1000}, // A4 - 1 second
 *     {880, 1000}  // A5 - 1 second
 * };
 * Song test_song = {
 *     .notes = test_notes,
 *     .length = sizeof(test_notes) / sizeof(Note)
 * };
 * printf("\nTesting PC Speaker...\n");
 * SongPlayer *player = create_song_player();
 * if (player)
 * {
 *     printf("Playing test notes...\n");
 *     player->play_song(&test_song);
 *     printf("Test complete\n");
 * }
 */