#include "matrix.h"
#include "printf.h"
#include "pit.h"
#include "memory.h"

// External terminal buffer from printf.h
extern unsigned short* terminal_buffer;
extern int cursor_x, cursor_y;

// Static buffer for raindrops instead of dynamic memory
static matrix_raindrop_t raindrops[VGA_WIDTH];
static unsigned short screen_backup[VGA_WIDTH * VGA_HEIGHT];
static int is_running = 0;
static uint32_t last_update_time = 0;
static uint32_t matrix_seed = 12345;

// Simple random number generator
uint8_t matrix_rand() {
    matrix_seed = (matrix_seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return (uint8_t)(matrix_seed & 0xFF);
}

// Initialize the Matrix rain animation
void matrix_init() {
    // Initialize all raindrops
    for (int i = 0; i < VGA_WIDTH; i++) {
        raindrops[i].length = 5 + (matrix_rand() % 10);    // Random length between 5 and 15
        raindrops[i].position = -(matrix_rand() % 20);     // Start above screen at different positions
        raindrops[i].speed = 1 + (matrix_rand() % 3);      // Random speed
        raindrops[i].tick_counter = 0;
        raindrops[i].active = 1;
        
        // Fill with random characters
        for (int j = 0; j < VGA_HEIGHT; j++) {
            // Use a mix of characters - ASCII 33-126 are visible characters
            raindrops[i].chars[j] = 33 + (matrix_rand() % 93);
            
            // Head of the raindrop is brighter
            if (j == 0) {
                raindrops[i].colors[j] = MATRIX_WHITE;      // White head
            } else if (j < 3) {
                raindrops[i].colors[j] = MATRIX_BRIGHT_GREEN;  // Bright green for top chars
            } else {
                raindrops[i].colors[j] = MATRIX_DARK_GREEN;    // Dark green for the rest
            }
        }
    }
    
    // Back up the current screen content to restore later
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        screen_backup[i] = terminal_buffer[i];
    }
    
    last_update_time = get_current_tick();
}

// Draw the current state of raindrops to the screen
void matrix_draw() {
    // Clear screen with black
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = 0;  // Black space
        }
    }

    // Draw each raindrop
    for (int i = 0; i < VGA_WIDTH; i++) {
        if (!raindrops[i].active) continue;
        
        // Calculate visible part of the raindrop
        int start_y = raindrops[i].position;
        if (start_y < 0) start_y = 0;
        
        // Draw each character of the raindrop that fits on screen
        for (int j = 0; j < raindrops[i].length; j++) {
            int y = raindrops[i].position - j;
            if (y >= 0 && y < VGA_HEIGHT) {
                // Only draw if within screen bounds
                int char_index = j % VGA_HEIGHT;  // Ensure we don't go out of bounds
                char display_char = raindrops[i].chars[char_index];
                uint8_t color = raindrops[i].colors[char_index];
                
                // Place character with color attribute in terminal buffer
                terminal_buffer[y * VGA_WIDTH + i] = (color << 8) | display_char;
                
                // Occasionally change characters for animation effect
                if (matrix_rand() % 20 == 0) {
                    raindrops[i].chars[char_index] = 33 + (matrix_rand() % 93);
                }
            }
        }
    }
}

// Update the positions of all raindrops
void matrix_update() {
    // Check if enough time has passed since last update (frame rate control)
    uint32_t current_time = get_current_tick();
    if (current_time - last_update_time < 200) {  // Update every 200ms (5 fps) - slower to reduce processing
        return;
    }
    last_update_time = current_time;
    
    // Update positions of all raindrops
    for (int i = 0; i < VGA_WIDTH; i++) {
        if (!raindrops[i].active) {
            // Small chance to reactivate
            if (matrix_rand() % 20 == 0) {
                raindrops[i].active = 1;
                raindrops[i].position = -(matrix_rand() % 15);
                raindrops[i].length = 5 + (matrix_rand() % 10);
                raindrops[i].speed = 1 + (matrix_rand() % 3);
            }
            continue;
        }
        
        raindrops[i].tick_counter++;
        if (raindrops[i].tick_counter >= raindrops[i].speed) {
            raindrops[i].tick_counter = 0;
            raindrops[i].position++; // Move raindrop down
            
            // If raindrop has left the screen, reset it or deactivate
            if (raindrops[i].position - raindrops[i].length > VGA_HEIGHT) {
                if (matrix_rand() % 4 != 0) {
                    // Reset raindrop to start falling from top again
                    raindrops[i].position = -(matrix_rand() % 15);
                } else {
                    // Deactivate this raindrop for a while
                    raindrops[i].active = 0;
                }
            }
        }
    }
    
    // Draw the updated raindrops
    matrix_draw();
}

// Start the Matrix rain animation
void matrix_start() {
    matrix_init();
    
    is_running = 1;
    clear_screen();
    
    uint32_t frame_count = 0;
    const uint32_t MAX_FRAMES = 100;  // Run for about 20 seconds (at 5 fps)
    
    // Main animation loop with a maximum number of frames
    while (is_running && frame_count < MAX_FRAMES) {
        matrix_update();
        sleep_interrupt(200);  // 200ms delay between updates
        frame_count++;
    }
    
    matrix_restore_screen();
}

// Stop the Matrix rain animation
void matrix_stop() {
    is_running = 0;
}

// Restore the screen to its pre-Matrix state
void matrix_restore_screen() {
    // Restore original screen content
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        terminal_buffer[i] = screen_backup[i];
    }
    
    // Reset cursor position
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}
