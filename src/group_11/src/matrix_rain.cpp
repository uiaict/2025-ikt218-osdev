#include "matrix_rain.h"
#include "vga.h"
#include "common.h"
#include "libc/stdlib.h"
#include "libc/system.h"

static MatrixDrop drops[MAX_DROPS];
static uint32_t ticks = 0;

static const char matrix_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@#$%^&*";

static char get_random_char(void) {
    return matrix_chars[rand() % (sizeof(matrix_chars) - 1)];
}

extern "C" void init_matrix_rain(void) {
    for(int i = 0; i < MAX_DROPS; i++) {
        drops[i].x = rand() % SCREEN_WIDTH;
        drops[i].y = -(rand() % SCREEN_HEIGHT);
        drops[i].speed = 1 + (rand() % 2);
        drops[i].length = 3 + (rand() % 10);
        drops[i].character = get_random_char();
    }
}

extern "C" void update_matrix_rain(void) {
    ticks++;
    for(int i = 0; i < MAX_DROPS; i++) {
        if(ticks % drops[i].speed == 0) {
            drops[i].y++;
            
            if(drops[i].y > SCREEN_HEIGHT) {
                drops[i].y = -drops[i].length;
                drops[i].x = rand() % SCREEN_WIDTH;
                drops[i].character = get_random_char();
            }
        }
    }
}

extern "C" void render_matrix_rain(void) {
    clear_screen();
    
    for(int i = 0; i < MAX_DROPS; i++) {
        for(int j = 0; j < drops[i].length; j++) {
            int y = drops[i].y - j;
            if(y >= 0 && y < SCREEN_HEIGHT) {
                uint8_t color = (j == 0) ? 0x0A : 0x02;
                put_char_at(drops[i].character, drops[i].x, y, color);
            }
        }
    }
}