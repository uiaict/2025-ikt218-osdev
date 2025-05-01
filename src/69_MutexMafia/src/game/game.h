#ifndef GAME_H
#define GAME_H

#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "../monitor/monitor.h"  // For draw_char_at
#include "../io/keyboard.h"       // For keyboard_key_pressed, keyboard_read
#include "../pit/pit.h"         // For sleep_interrupt
#include "../music/songplayer.h"       // For sound_beep
#include "../io/printf.h"         // For mafiaPrint


// Constants
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define PIPE_WIDTH 3
#define GAP_HEIGHT 5
#define GRAVITY 1
#define FLAP_STRENGTH -2
#define PIPE_SPEED 1
#define FRAME_DELAY_MS 60

// Structs
typedef struct {
    int y;
    int velocity;
} Bird;

typedef struct {
    int x;
    int gap_y;
} Pipe;

// Functions
void play_game();
void reset_game();
void handle_game_input();
void update_game();
void draw_game();

#endif // GAME_H