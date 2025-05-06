// src/games/snake.c

#include "libc/snake.h"
#include "libc/keyboard.h"
#include "libc/pit.h"
#include "libc/song.h"       // for Note and Song definitions
#include "libc/teminal.h"
#include <libc/stdint.h>
#include <libc/stdbool.h>

// VGA text-mode memory address
#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH   BOARD_WIDTH

// Border thickness
#define BORDER    2
#define MIN_X     BORDER
#define MIN_Y     BORDER
#define MAX_X     (BOARD_WIDTH  - BORDER - 1)
#define MAX_Y     (BOARD_HEIGHT - BORDER - 1)

// Speed constants (ms per frame)
#define INITIAL_SPEED_MS 150
#define SPEED_DECR_MS     15
#define MIN_SPEED_MS      15

// Forward declarations
static bool hits_body(int x, int y);
static int  snake_rand(int max);
static void place_apple(void);
static void game_over_screen(void);
static void play_death_jingle(void);

// Low-level speaker controls (from song.c)
extern void sound_start(uint32_t hz);
extern void sound_stop(void);

// Mario‐death jingle (no logging)
static Note death_notes[] = {
    { E5, 200 }, { D5, 200 }, { C5, 400 }
};
static Song death_song = {
    .notes  = death_notes,
    .length = sizeof(death_notes)/sizeof(death_notes[0])
};

// Play the death song without printing any Note info
static void play_death_jingle(void) {
    for (uint32_t i = 0; i < death_song.length; i++) {
        Note *n = &death_song.notes[i];
        sound_start(n->frequency);
        sleep_interrupt(n->duration);
        sound_stop();
    }
}

// Game state
static int     snake_x[SNAKE_MAX_LEN], snake_y[SNAKE_MAX_LEN];
static int     snake_len;
static Direction dir;
static int     apple_x, apple_y;
static uint32_t speed_ms, rnd_seed;

// Simple LCG for pseudo‐random
static int snake_rand(int max) {
    rnd_seed = rnd_seed * 1103515245 + 12345;
    return (rnd_seed >> 16) % max;
}

// Does (x,y) collide with the snake body?
static bool hits_body(int x, int y) {
    for (int i = 0; i < snake_len; i++)
        if (snake_x[i] == x && snake_y[i] == y)
            return true;
    return false;
}

// Place an apple in the inner area, avoiding the snake
static void place_apple(void) {
    do {
        apple_x = snake_rand(MAX_X - MIN_X + 1) + MIN_X;
        apple_y = snake_rand(MAX_Y - MIN_Y + 1) + MIN_Y;
    } while (hits_body(apple_x, apple_y));
}

// Show “GAME OVER” + 3…2…1 countdown
static void game_over_screen(void) {
    uint16_t* buf = (uint16_t*)VGA_ADDRESS;
    const uint8_t fg = 15, bg = 0;

    // Clear screen
    for (int y = 0; y < BOARD_HEIGHT; y++)
        for (int x = 0; x < BOARD_WIDTH; x++)
            buf[y*VGA_WIDTH + x] = (uint16_t)' ' | ((bg<<4|fg) << 8);

    // ASCII ART BANNER
    static const char* banner[] = {
        "  ____    _    __  __ _____ ____   ",
        " / ___|  / \\  |  \\/  | ____|  _ \\  ",
        "| |  _  / _ \\ | |\\/| |  _| | |_) | ",
        "| |_| |/ ___ \\| |  | | |___|  _ <  ",
        " \\____/_/   \\_\\_|  |_|_____|_| \\_\\ "
    };
    int bh = 5, bw = 0;
    while (banner[0][bw]) bw++;
    int sy = (BOARD_HEIGHT - bh)/2 - 1;
    int sx = (BOARD_WIDTH  - bw)/2;
    for (int i = 0; i < bh; i++)
        for (int j = 0; j < bw; j++)
            buf[(sy+i)*VGA_WIDTH + (sx+j)] =
                (uint16_t)banner[i][j] | ((bg<<4|fg) << 8);

    // Countdown
    const char* prefix = "Restarting in ";
    int plen = 0;
    while (prefix[plen]) plen++;
    int ly = sy + bh + 2;

    for (int count = 3; count >= 1; count--) {
        char msg[16];
        for (int k = 0; k < plen; k++) msg[k] = prefix[k];
        msg[plen] = '0' + count;
        int mlen = plen + 1;
        int lx = (BOARD_WIDTH - mlen)/2;

        // Clear line
        for (int x = 0; x < BOARD_WIDTH; x++)
            buf[ly*VGA_WIDTH + x] = (uint16_t)' ' | ((bg<<4|fg)<<8);

        // Draw countdown
        for (int k = 0; k < mlen; k++)
            buf[ly*VGA_WIDTH + lx + k] =
                (uint16_t)msg[k] | ((bg<<4|fg)<<8);

        sleep_interrupt(1000);
    }
}

// Initialize or reset the game
static void init_game(void) {
    snake_len = 3;
    int mx = (MIN_X + MAX_X)/2, my = (MIN_Y + MAX_Y)/2;
    snake_x[0]=mx; snake_y[0]=my;
    snake_x[1]=mx-1; snake_y[1]=my;
    snake_x[2]=mx-2; snake_y[2]=my;
    dir      = DIR_RIGHT;
    speed_ms = INITIAL_SPEED_MS;
    rnd_seed = get_tick();
    place_apple();
}

// Draw border, apple, snake, empty background
static void draw_frame(void) {
    uint16_t* buf = (uint16_t*)VGA_ADDRESS;
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            char    ch;
            uint8_t fg, bgc;

            if (x < BORDER || x > BOARD_WIDTH - BORDER - 1 ||
                y < BORDER || y > BOARD_HEIGHT - BORDER - 1) {
                ch  = ' '; fg = 0; bgc = 1;
            }
            else if (x == apple_x && y == apple_y) {
                ch  = '@'; fg = 4; bgc = 0;
            }
            else if (hits_body(x,y)) {
                ch  = 'O'; fg = 2; bgc = 0;
            }
            else {
                ch  = ' '; fg = 0; bgc = 0;
            }

            buf[y*VGA_WIDTH + x] =
                (uint16_t)ch | (((bgc<<4|fg)) << 8);
        }
    }
}

// Move the snake, check collisions & apples
static void update_snake(void) {
    int nx = snake_x[0], ny = snake_y[0];
    switch (dir) {
      case DIR_UP:    ny--; break;
      case DIR_DOWN:  ny++; break;
      case DIR_LEFT:  nx--; break;
      case DIR_RIGHT: nx++; break;
    }

    // Collision?
    if (nx < MIN_X || nx > MAX_X ||
        ny < MIN_Y || ny > MAX_Y ||
        hits_body(nx,ny))
    {
        play_death_jingle();      // no logs
        game_over_screen();
        init_game();
        return;
    }

    // Advance body
    for (int i = snake_len; i > 0; i--) {
        snake_x[i] = snake_x[i-1];
        snake_y[i] = snake_y[i-1];
    }
    snake_x[0] = nx;
    snake_y[0] = ny;

    // Ate apple?
    if (nx == apple_x && ny == apple_y) {
        if (snake_len < SNAKE_MAX_LEN) snake_len++;
        speed_ms = (speed_ms > MIN_SPEED_MS + SPEED_DECR_MS)
                   ? speed_ms - SPEED_DECR_MS
                   : MIN_SPEED_MS;
        sound_start(880);
        sleep_interrupt(100);
        sound_stop();
        place_apple();
    }
}

// Main loop: never returns
void snake_run(void) {
    init_game();
    while (1) {
        uint32_t t0 = get_tick();

        // Input
        char k = get_last_key();
        if (k=='w' && dir!=DIR_DOWN)  dir=DIR_UP;
        if (k=='s' && dir!=DIR_UP)    dir=DIR_DOWN;
        if (k=='a' && dir!=DIR_RIGHT) dir=DIR_LEFT;
        if (k=='d' && dir!=DIR_LEFT)  dir=DIR_RIGHT;

        update_snake();
        draw_frame();

        // Frame cap
        uint32_t dt = get_tick() - t0;
        if (dt < speed_ms)
            sleep_interrupt(speed_ms - dt);
    }
}
