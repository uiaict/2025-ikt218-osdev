#ifndef GAME_H
#define GAME_H

#include "libc/system.h"

#include "../monitor/monitor.h"  // For draw_char_at
#include "../io/keyboard.h"      // For keyboard_key_pressed, keyboard_read
#include "../pit/pit.h"          // For sleep_interrupt
#include "../music/songplayer.h" // For sound_beep
#include "../io/printf.h"        // For mafiaPrint
#include "../memory/malloc.h"    // For malloc/free

// Constants
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define PIPE_WIDTH 3
#define GAP_HEIGHT 5
#define GRAVITY 1
#define FLAP_STRENGTH -2
#define PIPE_SPEED 1
#define FRAME_DELAY_MS 60
#define MAX_HIGHSCORES 5

// Structs
typedef struct
{
    int y;
    int velocity;
} Bird;

typedef struct
{
    int x;
    int gap_y;
} Pipe;

typedef struct
{
    int scores[MAX_HIGHSCORES];
    int count;
} HighscoreTable;

// Functions
void play_game();
void reset_game();
void handle_game_input();
void update_game();
void draw_game();
void init_highscores();
void insert_highscore(int new_score);
void print_highscores();
void play_start_sound();
void game_over_sound();

#endif // GAME_H