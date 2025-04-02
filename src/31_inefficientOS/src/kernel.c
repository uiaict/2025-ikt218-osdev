#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "gdt.h"
#include "terminal.h"
#include "common.h"
#include "idt.h"
#include "interrupts.h"
#include "memory.h"
#include "pit.h"
#include "song.h"

extern void custom_isrs_init();
extern void keyboard_init();

// From linker.ld - marks end of kernel code
extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Convert a number to hex string
void uint_to_hex(uint32_t num, char* str) {
    const char hex_chars[] = "0123456789ABCDEF";
    for(int i = 7; i >= 0; i--) {
        str[i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    str[8] = '\0';
}

// Convert integer to string
void int_to_str(int num, char* str) {
    int i = 0;
    bool is_negative = false;
    
    // Handle 0 case
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    // Handle negative numbers
    if (num < 0) {
        is_negative = true;
        num = -num;
    }
    
    // Convert digits
    while (num != 0) {
        str[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    // Add negative sign if needed
    if (is_negative) {
        str[i++] = '-';
    }
    
    str[i] = '\0';
    
    // Reverse the string
    int j;
    for (j = 0; j < i/2; j++) {
        char temp = str[j];
        str[j] = str[i-j-1];
        str[i-j-1] = temp;
    }
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize terminal for output
    terminal_initialize();
    terminal_writestring("Terminal initialized\n");
   
    // Print magic number for debug
    char hex_str[9];
    uint_to_hex(magic, hex_str);
    terminal_writestring("Magic number: 0x");
    terminal_writestring(hex_str);
    terminal_writestring("\n");
   
    // Check multiboot magic
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        terminal_writestring("Invalid multiboot2 magic number!\n");
        return -1;
    }
   
    // Hello world with colors
    terminal_write_colored("Hello?\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    terminal_write_colored("Hello\n", VGA_COLOR_RED, VGA_COLOR_BLACK);
    terminal_write_colored("Hello\n", VGA_COLOR_BROWN, VGA_COLOR_BLACK);
    terminal_write_colored("Hello...\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
    terminal_write_colored("Is there anybody in there?\n", VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_write_colored("Just nod if you can hear me\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    terminal_write_colored("Is there anyone home?\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    terminal_writestring("Hello world\n");
   
    // Initialize GDT
    terminal_writestring("Initializing GDT...\n");
    gdt_init();
    terminal_writestring("GDT initialized!\n");
   
    // Initialize IDT
    terminal_writestring("Initializing IDT...\n");
    idt_init();
    terminal_writestring("IDT initialized!\n");
   
    // Initialize IRQs
    terminal_writestring("Initializing IRQ...\n");
    init_irq();
    terminal_writestring("IRQ initialized!\n");
   
    // Initialize custom ISRs
    terminal_writestring("Initializing custom ISRs...\n");
    custom_isrs_init();
   
    // Init keyboard
    terminal_writestring("Initializing keyboard...\n");
    keyboard_init();
    
    // ASSIGNMENT 4 PART 1: Memory Management
    terminal_writestring("Initializing kernel memory...\n");
    init_kernel_memory(&end);
    
    terminal_writestring("Initializing paging...\n");
    init_paging();
    
    terminal_writestring("Memory layout:\n");
    print_memory_layout();
    
    // Test memory allocation
    terminal_writestring("Testing memory allocation...\n");
    void* memory1 = malloc(12345);
    void* memory2 = malloc(54321);
    void* memory3 = malloc(13331);
    
    terminal_writestring("Memory allocated at: ");
    uint_to_hex((uint32_t)memory1, hex_str);
    terminal_writestring(hex_str);
    terminal_writestring(", ");
    uint_to_hex((uint32_t)memory2, hex_str);
    terminal_writestring(hex_str);
    terminal_writestring(", ");
    uint_to_hex((uint32_t)memory3, hex_str);
    terminal_writestring(hex_str);
    terminal_writestring("\n");
    
    // ASSIGNMENT 4 PART 2: PIT
    terminal_writestring("Initializing PIT...\n");
    init_pit();
    
// ASSIGNMENT 5: Music player
terminal_write_colored("\n=== ANOTHER BRICK IN THE WALL ===\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    
// Create song player
SongPlayer* player = create_song_player();
if (player) {
    // Get song structure
    extern Song another_brick;
    
    // Play the song
    terminal_write_colored("Playing Another Brick in the Wall...\n", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    player->play_song(another_brick);
    terminal_writestring("Song finished.\n");
    
    // Clean up
    free(player);
    terminal_write_colored("Music playback complete!\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
} else {
    terminal_writestring("Failed to create song player.\n");
}
    
    // Test sleep functions
    uint32_t counter = 0;
    char counter_str[10];
    
    // Print header once
    terminal_writestring("\n=== SLEEP TEST ===\n");
    terminal_writestring("Comparing busy-wait (HIGH CPU) vs interrupt (LOW CPU) sleep methods\n");
    terminal_writestring("Each cycle: 1 second busy-wait followed by 1 second interrupt-based\n\n");
    
    // Main loop
    while(1) {
        // Update counter
        int_to_str(counter, counter_str);
        
        // Start of test cycle
        terminal_writestring("Cycle [");
        terminal_writestring(counter_str);
        terminal_writestring("]: ");
        
        // Busy wait sleep
        terminal_write_colored("BUSY", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
        sleep_busy(1000);
        terminal_write_colored(" -> ", VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        
        // Interrupt sleep
        terminal_write_colored("INT", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
        sleep_interrupt(1000);
        terminal_writestring(" Complete\n");
        
        // Increment counter
        counter++;
        
        // Periodically reallocate memory to test free()
        if (counter % 5 == 0) {
            terminal_writestring("  [Memory test: freeing and reallocating...]\n");
            free(memory2);
            memory2 = malloc(1000);
        }
    }
    
    return 0;
}