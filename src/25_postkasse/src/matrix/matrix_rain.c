#include "matrix_rain.h"
#include "libc/monitor.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/string.h"
#include "libc/pit.h"

#define GREEN_ON_BLACK 0x02
extern volatile char last_key;
extern cursor_x;
extern cursor_y;

//creates a pointer to the VGA addess
volatile char* const video = (volatile char*)VGA_ADDR;

//Almost random number generator
static uint32_t seed = 12345;
uint8_t random_number_generator() {
    seed = (seed * 1103515245 + 12345) >> 16;
    return (uint8_t)(seed & 0xFF);
}

void matrix_rain_step() {
    for (int collumn = 0; collumn < VGA_WIDTH; collumn++) {
        //30% chance to generate a character
        if (random_number_generator() % 10 < 3) {
            //Select a random character from the ASCII range 33-126
            char ch = 33 + random_number_generator() % 94;
            monitor_put_for_matrix(ch, collumn, 1, GREEN_ON_BLACK);
        } else {
            // Leave space
            monitor_put_for_matrix(' ', collumn, 1, GREEN_ON_BLACK);
        }
    }

    // Copies the character and the color from the row above to the current row
    for (int row = VGA_HEIGHT - 1; row > 1; --row) {
        for (int collumn = 0; collumn < VGA_WIDTH; ++collumn) {
            int idx = (row * VGA_WIDTH + collumn) * 2;
            int above_idx = ((row - 1) * VGA_WIDTH + collumn) * 2;
            video[idx] = video[above_idx];
            video[idx + 1] = video[above_idx + 1];
        }
    }
}

void clear_screen() {

    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            monitor_put(' '); 
        }
    }

    cursor_x = 0;
    cursor_y = 0;

}

void draw_title() {
    const char* title = "==== Matrix Rain, Press 'Q' to Quit ====";

    int titleLen = strlen(title);
    
    int title_col = (VGA_WIDTH - titleLen) / 2;
    
    // Draw title at row 0
    for (int i = 0; i < titleLen; i++) {
        monitor_put_for_matrix(title[i], title_col + i, 0, WHITE_ON_BLACK);
    }

}

//Inifitirte loop that runs the matrix rain
void run_matrix_rain() {
    clear_screen(); // Clear the screen before starting
    draw_title(); // <-- draw title first

    while (1) {
        matrix_rain_step();
        sleep_interrupt(100);

        if (last_key == 'q' || last_key == 'Q') {
            monitor_write("\nExiting Matrix Rain...\n");
            last_key = 0; // Clear last_key after quitting rain
            clear_screen();
            break;
        }
    }
}