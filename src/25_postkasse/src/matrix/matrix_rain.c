#include "matrix_rain.h"
#include "libc/monitor.h"
#include "libc/stdint.h"
#include "libc/string.h"
#include "libc/pit.h"

#define GREEN_ON_BLACK 0x02

extern volatile char last_key;
extern int cursor_x;
extern int cursor_y;

// Just a simple number to start our "random" numbers
unsigned int seed = 96024;

//Make a random number
uint8_t random_number() {
    seed = (seed * 1103515245 + 12345) >> 16;
    return seed % 256;
}

//Declare the attributes every collumn has
int drop_y[VGA_WIDTH];       // How far the drop has fallen
int drop_speed[VGA_WIDTH];   // How fast each drop falls
int drop_timer[VGA_WIDTH];   // Time before the next drop step
int drop_length[VGA_WIDTH];  // How long the trail is

//clear the whole screen
void clear_screen() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            monitor_put(' '); 
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

//Draw the title at the top
void draw_title() {
    cursor_x = 20;
    monitor_write("==== MATRIX RAIN - PRESS 'Q' TO QUIT ====");
    cursor_x = 0;
}

//Create thje drops
void create_drop_attributes() {
    for (int x = 0; x < VGA_WIDTH; x++) {
        drop_y[x] = random_number() % VGA_HEIGHT;  //Random roew start position
        drop_speed[x] = (random_number() % 5) + 1; // Random speed
        drop_timer[x] = drop_speed[x];             //timer is bade on the sped
        drop_length[x] = (random_number() % 6) + 4;// Random length from 4 to 9 rows
    }
}

// Make the Matrix rain move
void matrix_rain_step() {
    //go throuhg every collumn
    for (int x = 0; x < VGA_WIDTH; x++) {
        drop_timer[x]--; // Decrease the timer

        //If the timer is up, draw the drop
        if (drop_timer[x] <= 0) {
            drop_timer[x] = drop_speed[x];

            // Draw the drop
            for (int i = 0; i < drop_length[x]; i++) {
                int row = drop_y[x] - i; 
                if (row >= 1 && row < VGA_HEIGHT) { 
                    char ch = 33 + random_number() % 94; // Random character from 33 to 126(These are printable)
                    uint8_t color = (i == 0) ? WHITE_ON_BLACK : GREEN_ON_BLACK; //First character is white the rest are green
                    monitor_put_with_color(ch, x, row, color);
                }
            }

            //Clear the trail behind
            int clear_row = drop_y[x] - drop_length[x];
            if (clear_row >= 1 && clear_row < VGA_HEIGHT) {
                monitor_put_with_color(' ', x, clear_row, GREEN_ON_BLACK);
            }

            //move drop down
            drop_y[x]++;
            if (drop_y[x] - drop_length[x] > VGA_HEIGHT + 5) {
                drop_y[x] = 1;
                drop_length[x] = (random_number() % 6) + 4;
                drop_speed[x] = (random_number() % 5) + 1;
            }
        }
    }
}

//Run forever until 'q' is pressed
void run_matrix_rain() {
    clear_screen();
    draw_title();
    create_drop_attributes();

    while (1) {
        matrix_rain_step();
        sleep_interrupt(50);

        if (last_key == 'q' || last_key == 'Q') {
            monitor_write("\nExiting Matrix Rain...\n");
            last_key = 0;
            clear_screen();
            break;
        }
    }
}
