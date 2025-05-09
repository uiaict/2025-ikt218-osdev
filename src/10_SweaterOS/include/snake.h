#ifndef SNAKE_H
#define SNAKE_H

#include "libc/stdint.h"
#include "libc/stdbool.h"

// Konstanter for spillet
#define SNAKE_MAX_LENGTH 100
#define BORDER_CHAR 219        // Blokktegn for ramme
#define SNAKE_CHAR 'O'         // Tegn for slangekropp
#define APPLE_CHAR '*'         // Tegn for eple
#define SNAKE_HEAD_CHAR '@'    // Tegn for slangehode

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

// Viser meny/instruksjoner for slangespillet
void show_snake_menu(void);

// Håndterer hovedspill-løkken for snake
void handle_snake_game(void);

// Tegner hele spillskjermen
void draw_game_screen(const GameState* state);

// Oppdaterer spilltilstanden
void update_game(GameState* state, Direction input_direction);

// Genererer en ny epleposisjon
void generate_apple(GameState* state);

// Sjekker om det er kollisjon
bool check_collision(const GameState* state);

// Flytter slangen i gjeldende retning
void move_snake(GameState* state);

// Prosesserer tastaturinput
Direction process_input(Direction current_direction, bool* quit_game);

// Hovedspillfunksjon for snake (kalles fra menyen)
void snake_game(void);

#endif // SNAKE_H 