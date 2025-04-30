#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include <gdt/gdt.h>
#include <libc/idt.h>
#include <libc/isr.h>
#include <print.h>
#include <putchar.h>
#include <libc/irq.h>
#include <keyboard.h>
#include "memory.h"  // Changed from <memory.h>
#include "libc/song.h"

extern uint32_t end; // Linker-provided symbol for end of kernel

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// A simple test handler for IRQ debugging
void test_irq_handler(registers_t regs) {
    printf("TEST IRQ HANDLER CALLED FOR IRQ %d\n", regs.int_no - 32);
    
}

SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    player->play_song = play_song_impl;
    return player;
}

void play_music() {
    // How to play music
    Song songs[] = {
        {starwars_theme, sizeof(starwars_theme) / sizeof(Note)}
    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song);

    SongPlayer* player = create_song_player();

    
    for(uint32_t i = 0; i < n_songs; i++) {
        printf("Playing Song...\n");
        player->play_song(&songs[i]);
        printf("Finished playing the song.\n");
    }
    
    free(player);
}


// Piano display definitions
#define PIANO_START_ROW 20          // Lower on the screen
#define PIANO_START_COL 15
#define KEY_WIDTH 6                 // Width of each key
#define KEY_HEIGHT 5                // Height of each key
#define NUM_KEYS 8

// Track which keys are currently pressed (1-8)
bool key_pressed[8] = {false};

// Function to draw the piano on screen
void draw_piano() {
    // Calculate total piano width including separators
    int total_width = (NUM_KEYS * KEY_WIDTH) + (NUM_KEYS - 1);
    
    // Center the piano horizontally on the screen (assuming 80 columns)
    uint16_t centered_col = (80 - (total_width + 2)) / 2;
    
    // Draw the piano title/instructions above the piano
    terminal_set_cursor_position(PIANO_START_ROW - 2, centered_col);
    printf("PIANO - Press keys 1-8 to play notes");
    
    // Draw the top border of the piano
    terminal_set_cursor_position(PIANO_START_ROW, centered_col);
    putchar('+');
    for (int i = 0; i < total_width; i++) {
        putchar('-');
    }
    putchar('+');
    
    // Draw keys and separators
    for (int row = 1; row <= KEY_HEIGHT; row++) {
        terminal_set_cursor_position(PIANO_START_ROW + row, centered_col);
        putchar('|');  // Left border
        
        for (int key = 0; key < NUM_KEYS; key++) {
            // Decide what to display in the key based on whether it's pressed
            if (key_pressed[key]) {
                // Key is pressed - fill with "#" symbols
                // For the middle row, show the key number among the "#" symbols
                if (row == KEY_HEIGHT / 2) {
                    char key_label = '1' + key;
                    // Calculate how many "#" to show before and after the key number
                    int hash_before = KEY_WIDTH / 3;
                    int hash_after = KEY_WIDTH - hash_before - 1;
                    
                    for (int h = 0; h < hash_before; h++) {
                        putchar('#');
                    }
                    putchar(key_label);
                    for (int h = 0; h < hash_after; h++) {
                        putchar('#');
                    }
                } 
                // Other rows just show "#" symbols when pressed
                else {
                    // Every other row gets filled with "#" when pressed
                    for (int w = 0; w < KEY_WIDTH; w++) {
                        putchar('#');
                    }
                }
            } else {
                // Key is not pressed - show normal key
                if (row == KEY_HEIGHT / 2) {
                    // Middle row with key number centered
                    char key_label = '1' + key;
                    int spaces_before = (KEY_WIDTH - 1) / 2;
                    int spaces_after = KEY_WIDTH - spaces_before - 1;
                    
                    for (int s = 0; s < spaces_before; s++) {
                        putchar(' ');
                    }
                    putchar(key_label);
                    for (int s = 0; s < spaces_after; s++) {
                        putchar(' ');
                    }
                } else {
                    // Empty space for key body
                    for (int w = 0; w < KEY_WIDTH; w++) {
                        putchar(' ');
                    }
                }
            }
            
            // Add separator between keys (but not after the last key)
            if (key < NUM_KEYS - 1) {
                putchar('|');
            }
        }
        
        putchar('|');  // Right border
    }
    
    // Draw the bottom border of the piano
    terminal_set_cursor_position(PIANO_START_ROW + KEY_HEIGHT + 1, centered_col);
    putchar('+');
    for (int i = 0; i < total_width; i++) {
        putchar('-');
    }
    putchar('+');
}

// Function to update key press visualization
void update_piano_key(int key_num, bool is_pressed) {
    if (key_num < 1 || key_num > 8) return;
    
    // Update key state
    key_pressed[key_num - 1] = is_pressed;
    
    // Redraw piano
    draw_piano();
}

// Hook this to the keyboard handler to visualize key presses
void on_key_press(uint8_t scancode, bool is_pressed) {
    // Map number keys 1-8 to piano keys
    if (scancode >= 0x02 && scancode <= 0x09) { // Scancodes for 1-8
        int key_num = scancode - 0x01; // Convert to 1-8
        update_piano_key(key_num, is_pressed);
    }
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // Initialize terminal for output
    terminal_initialize();

    printf("initializing kernel memory...\n");
    init_kernel_memory((uint32_t*)&end);
    printf("Kernel memory initialized\n");
    
    // Set up GDT before IDT
    printf("Initializing GDT...\n");
    gdt_install();

    // Initialize the IDT
    printf("Initializing IDT...\n");
    init_idt();
    
    printf("Initializing IRQ system...\n");
    init_irq();
    
    // Make sure interrupts are disabled while setting up handlers
    asm volatile("cli");
    
    // Initialize keyboard
    init_keyboard();
    
    // Initialize paging
    printf("Initializing paging...\n");
    init_paging();
    print_memory_layout();
    
    // Initialize PIT before enabling interrupts
    printf("Initializing PIT...\n");
    init_pit();
    
    printf("----------------------------------\n");
    printf("DEBUG: About to enable interrupts\n");
    
    // Enable interrupts after all handlers are registered
    asm volatile("sti");
    printf("Interrupts enabled\n");

    //play_music();

    int counter = 0;
    printf("Testing PIT with sleep functions...\n");
    
    // Clear the screen before drawing the piano
    terminal_initialize();
    
    // Draw the initial piano
    draw_piano();
    
    // Main loop
    while(1) {
        // The keyboard input is handled by interrupts
        // Piano keys will update visually when keys are pressed
    }

    return 0;
}