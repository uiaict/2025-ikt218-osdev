#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <libc/stdint.h>
#include <libc/stdbool.h>

// Maximum snake length
#define MAX_SNAKE_LENGTH 100

// Direction constants
#define DIR_UP    0
#define DIR_RIGHT 1
#define DIR_DOWN  2
#define DIR_LEFT  3

// Game area dimensions (considering terminal size)
#define GAME_WIDTH  40
#define GAME_HEIGHT 15

// Snake segment structure
typedef struct {
    int x, y;
} SnakeSegment;

// Function to start the snake game
void start_snake_game();

// Function to handle keyboard input in snake game
void snake_handle_input(char key);

// Function to play eating sound
void play_eating_sound();

// Function to check if the snake game is active
bool is_snake_game_active();

// Process any pending game updates
void process_pending_tasks();

// Add a debug function to force a game update
void force_snake_game_update();

#endif // SNAKE_GAME_H
