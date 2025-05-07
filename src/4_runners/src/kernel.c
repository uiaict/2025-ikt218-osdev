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
void sleep_busy(int milliseconds);


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
   Snake Game Background Music
   ========================================================================== */
static Note snake_music[] = {
    {.frequency = NOTE_C4, .duration = 150}, // C4 - Start low
    {.frequency = NOTE_E4, .duration = 150}, // E4 - Quick jump
    {.frequency = NOTE_G4, .duration = 300}, // G4 - Hold for emphasis
    {.frequency = NOTE_E4, .duration = 150}, // E4 - Descend
    {.frequency = NOTE_D4, .duration = 150}, // D4 - Continue descending
    {.frequency = NOTE_E4, .duration = 150}, // E4 - Bounce back
    {.frequency = NOTE_G4, .duration = 150}, // G4 - Ascend again
    {.frequency = NOTE_C5, .duration = 150}  // C5 - End high
};

static Song snake_song = {
    .notes = snake_music,
    .length = sizeof(snake_music) / sizeof(Note)
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

//   int counter = 0;
//   while (1) {
//       printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
//       sleep_busy(1000);
//       printf("[%d]: Slept using busy-waiting.\n", counter++);
 
//       printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
//       sleep_interrupt(1000);
//       printf("[%d]: Slept using interrupts.\n", counter++);
//   }

    terminal_write("Initializing Programmable Interval Timer (PIT)...\n");
    init_pit();
    verify_pit_channel2();
    delay(5);

    // Add after PIT initialization
    terminal_write("Initializing PC Speaker...\n");
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
        {.frequency = NOTE_C4, .duration = 500}, // C4
        {.frequency = NOTE_E4, .duration = 500}, // E4
        {.frequency = NOTE_G4, .duration = 500}, // G4
        {.frequency = NOTE_C5, .duration = 500}  // C5
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
    delay(10);
    speaker_control(false); // Turn off speaker

    // Enable interrupts globally
    asm volatile("sti");

menu_start:
    /* ----------------------------------------------------------------------
       Display Menu
       ---------------------------------------------------------------------- */
    terminal_clear();
    printf("Welcome to 4_runners Os!\n");
    printf("================\n\n");
    printf("Available Options:\n");
    printf("1. Snake Game\n");
    printf("2. Memory Layout\n");
    printf("3. Play music\n\n");
    printf("Press 1-3 to select option...\n");

    /* ----------------------------------------------------------------------
       Main Loop
       ---------------------------------------------------------------------- */
    while (1)
    {
        char key = keyboard_getchar();
       
        if (key != 0) // Only play sound if a key is pressed
        {
            static Note feedback_note[] = {
                {.frequency = NOTE_E4, .duration = 200}  // E4 - Short feedback sound
            };
            Song feedback_sound = {
                .notes = feedback_note,
                .length = sizeof(feedback_note) / sizeof(Note)
            };
            SongPlayer *feedback_player = create_song_player();
            if (feedback_player)
            {
                feedback_player->play_song(&feedback_sound);
                delay_ms(5); // Ensure the note finishes playing
            }
        }
        if (key == '1')
        {
            terminal_clear();
            terminal_set_color(COLOR_TITLE);
            printf("\n");
            printf("  ____       _    _         _          _   __     _____ \n");
            printf(" / ___|     | \\ | |       / \\       | | / /    | ____|\n");
            printf(" \\_ \\      |  \\| |      / _ \\      |  | /     |  _|  \n");
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
                // Start background music for Snake game
                start_background_music(&snake_song, true);

                bool running = true;
                while (running)
                {
                    if (!get_game_mode())
                    {
                        // Game mode was disabled (e.g., by pressing ESC in snake.c)
                        running = false;
                        stop_background_music(); // Stop music when exiting
                        goto menu_start;         // Return to the menu
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
        else if (key == '3')
        {
            terminal_clear();
            terminal_set_color(COLOR_FOOD);
            printf("Playing 'Twinkle, twinkle, little star' ...\n");
            terminal_set_color(0x07); // Reset color
            printf("=====================\n\n");
            delay(5);
            speaker_control(true);
            static Note test_melody[] = {
                // First verse: "Twinkle, twinkle, little star"
                {.frequency = NOTE_C4, .duration = 500},  // Twin-
                {.frequency = NOTE_C4, .duration = 500},  // -kle
                {.frequency = NOTE_G4, .duration = 500},  // twin-
                {.frequency = NOTE_G4, .duration = 500},  // -kle
                {.frequency = NOTE_A4, .duration = 500},  // lit-
                {.frequency = NOTE_A4, .duration = 500},  // -tle
                {.frequency = NOTE_G4, .duration = 1000}, // star
                {.frequency = 0,      .duration = 300},   // Rest
        
                // Second line: "How I wonder what you are"
                {.frequency = NOTE_F4, .duration = 500},  // How
                {.frequency = NOTE_F4, .duration = 500},  // I
                {.frequency = NOTE_E4, .duration = 500},  // won-
                {.frequency = NOTE_E4, .duration = 500},  // -der
                {.frequency = NOTE_D4, .duration = 500},  // what
                {.frequency = NOTE_D4, .duration = 500},  // you
                {.frequency = NOTE_C4, .duration = 1000}, // are
                {.frequency = 0,      .duration = 300},   // Rest
        
                // Third line: "Up above the world so high"
                {.frequency = NOTE_G4, .duration = 500},  // Up
                {.frequency = NOTE_G4, .duration = 500},  // a-
                {.frequency = NOTE_F4, .duration = 500},  // -bove
                {.frequency = NOTE_F4, .duration = 500},  // the
                {.frequency = NOTE_E4, .duration = 500},  // world
                {.frequency = NOTE_E4, .duration = 500},  // so
                {.frequency = NOTE_D4, .duration = 1000}, // high
                {.frequency = 0,      .duration = 300},   // Rest
        
                // Fourth line: "Like a diamond in the sky"
                {.frequency = NOTE_G4, .duration = 500},  // Like
                {.frequency = NOTE_G4, .duration = 500},  // a
                {.frequency = NOTE_F4, .duration = 500},  // dia-
                {.frequency = NOTE_F4, .duration = 500},  // -mond
                {.frequency = NOTE_E4, .duration = 500},  // in
                {.frequency = NOTE_E4, .duration = 500},  // the
                {.frequency = NOTE_D4, .duration = 1000}, // sky
                {.frequency = 0,      .duration = 300},   // Rest
        
             
            };
            Song test_song = {
                .notes = test_melody,
                .length = sizeof(test_melody) / sizeof(Note)
            };
            printf("\nTesting PC Speaker...\n");
            SongPlayer *player = create_song_player();
            if (player)
            {
                printf("Playing melody...\n");
                player->play_song(&test_song);
                printf("Melody complete.\n");
            }
            else
            {
                printf("Failed to create song player.\n");
            }
            delay(5);
            printf("\nPress any key to return to menu...\n");
    
            speaker_control(false);
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

