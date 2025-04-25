#ifndef SNAKE_H
#define SNAKE_H

#include "libc/stdint.h"
#include "libc/stdbool.h"

// Constants for the game
#define SNAKE_MAX_LENGTH 100
#define BORDER_CHAR 219        // Block character for border
#define SNAKE_CHAR 'O'         // Character for snake body
#define APPLE_CHAR '*'         // Character for apple
#define SNAKE_HEAD_CHAR '@'    // Character for snake head

// Direction definitions
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

// Position structure
typedef struct {
    int x;
    int y;
} Position;

// Snake structure
typedef struct {
    Position segments[SNAKE_MAX_LENGTH];
    int length;
    Direction direction;
    bool is_alive;
} Snake;

// Game state
typedef struct {
    Snake snake;
    Position apple;
    uint32_t score;
    uint32_t game_speed;
    bool game_over;
} GameState;

// Initialize the snake game
void snake_init(void);

// Show the snake game menu/instructions
void show_snake_menu(void);

// Handle the main snake game loop
void handle_snake_game(void);

// Draw the entire game screen
void draw_game_screen(const GameState* state);

// Update the game state
void update_game(GameState* state, Direction input_direction);

// Generate a new apple position
void generate_apple(GameState* state);

// Check if there's a collision
bool check_collision(const GameState* state);

// Move the snake in the current direction
void move_snake(GameState* state);

// Process keyboard input
Direction process_input(Direction current_direction, bool* quit_game);

#endif // SNAKE_H 