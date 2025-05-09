#include "monitor.h"
#include "matrix_rain.h"


static unsigned int seed = 123456789;
int rain_enabled = 0;


unsigned int rand_simple() {
    seed = seed * 1103515245 + 12345;
    return (seed / 65536) % 32768;
}

char random_char() {
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int index = rand_simple() % (sizeof(charset) - 1); 
    return charset[index];
}


#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

int rain_positions[SCREEN_WIDTH];

void rain_init() {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        rain_positions[x] = rand_simple() % (SCREEN_HEIGHT * 2) - SCREEN_HEIGHT;
    }
}



void draw_char(int x, int y, char c) {
    uint8_t color = (0 /*black*/ << 4) | (10 /*light green*/ & 0x0F);
    monitor_putentryat(c, color, x, y);
}


// Clear character at (x, y)
void clear_char(int x, int y) {
    draw_char(x, y, ' ');
}


void rain_update() {
    if (!rain_enabled) {
        return;
    }

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        int y = rain_positions[x];

        if (y < SCREEN_HEIGHT) {
            char c = random_char();
            draw_char(x, y, c);
        }

        // Clear the character behind (optional for better look)
        if (y > 0) {
            clear_char(x, y - 1);
        }

        rain_positions[x]++;

        if (rain_positions[x] >= SCREEN_HEIGHT) {
            rain_positions[x] = 0;
        }
    }
}
