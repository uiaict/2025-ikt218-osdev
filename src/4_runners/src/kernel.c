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
#include "song.h"
#include <multiboot2.h>

// External symbols
extern char end;

// Function declarations
void terminal_write(const char *str);
void terminal_put_char(char c);
void initkeyboard(void);
extern char keyboard_getchar(void);

// Multiboot structure
struct multiboot_info
{
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Helper to print hex
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

// Simple add function for testing
int compute(int a, int b)
{
    return a + b;
}

// Sample struct for testing multiboot cast
typedef struct
{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e[6];
} myStruct;

void verify_pit_channel2() {
    // Read back command
    outb(PIT_CMD_PORT, 0xE8);  // Read-back command for channel 2
    
    // Read status
    uint8_t status = inb(PIT_CHANNEL2_PORT);
    printf("PIT Channel 2 Status: 0x%02x\n", status);
    
    // Read current count
    uint8_t count_low = inb(PIT_CHANNEL2_PORT);
    uint8_t count_high = inb(PIT_CHANNEL2_PORT);
    uint16_t count = (count_high << 8) | count_low;
    
    printf("PIT Channel 2 Count: %u\n", count);
}

void test_pc_speaker() {
    terminal_write("Testing PC speaker...\n");
    
    // Test a range of frequencies
    uint32_t test_frequencies[] = {
        440,  // A4
        880,  // A5
        1760, // A6
        220   // A3
    };
    
    for (int i = 0; i < 4; i++) {
        printf("Testing frequency %u Hz\n", test_frequencies[i]);
        set_speaker_frequency(test_frequencies[i]);
        sleep_busy(500);  // 500ms per note
        set_speaker_frequency(0);
        sleep_busy(100);  // 100ms gap
    }
    
    terminal_write("Test complete.\n");
}

/* --------------------------------------------------------------------------
   KERNEL MAIN LOOP
   -------------------------------------------------------------------------- */
int kernel_main(void)
{
    // terminal_write("Hello, World!\n");

    // // Trigger some test interrupts
    // asm volatile("int $0");
    // asm volatile("int $1");
    // asm volatile("int $2");

    // Keyboard
    terminal_write("Initializing keyboard...\n");
    initkeyboard();
    terminal_write("Keyboard initialized.\n");

    // Move to new line
    int row, col;
    terminal_get_cursor(&row, &col);
    terminal_set_cursor(row + 1, 0);

    // Initialize PIT
    init_pit();
    asm volatile("sti"); // Enable interrupts globally

    int counter = 0;
    while (1) {
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);
    }
    // Try out the music

    test_pc_speaker();  // Test PC speaker
    verify_pit_channel2(); 

    static Note test_notes[] = {
        {440, 1000},   // A4 for 1 second
        {494, 1000},   // B4 for 1 second
        {523, 1000},   // C5 for 1 second
        {0,   500},    // Longer rest
        {587, 1000},   // D5 for 1 second
    };
    
    Song test_song = {
        .notes = test_notes,
        .note_count = sizeof(test_notes)/sizeof(Note)
    };

    SongPlayer* player = create_song_player();
    // Play the test tune once:
    terminal_write("Playing a test tune...\n");
    player->play_song(&test_song);
    terminal_write("Finished playing tune.\n");

    while (1)
    {
        asm volatile("hlt");
    }

    return 0;
}

/* --------------------------------------------------------------------------
   BOOT ENTRY
   -------------------------------------------------------------------------- */
int main(uint32_t structAddr, uint32_t magic, struct multiboot_info *mb_info_addr)
{
    terminal_write("System initializing...\n");

    // Kernel end
    terminal_write("Kernel end = ");
    print_hex((uint32_t)&end);
    terminal_write("\n");

    // Setup core components
    gdt_init();
    idt_init();
    irq_init();

    // Memory setup
    terminal_write("Initializing memory...\n");
    init_kernel_memory((uint32_t *)&end);
    init_paging();

    // Initial layout
    terminal_write("\nInitial Memory Layout:\n");
    print_memory_layout();

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

    // Testing multiboot struct
    myStruct *myStructPtr = (myStruct *)structAddr;
    int result = compute(1, 2);

    terminal_write("System initialized\n\n");
    return kernel_main();
}
