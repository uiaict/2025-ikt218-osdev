#ifndef SNAKES_H
#define SNAKES_H

#define SNAKE_GAME_WIDTH 70
#define SNAKE_GAME_HEIGHT 20
#define SNAKE_TILE_SIZE 1
#define SNAKE_COLOR 0x0A // Green
#define FOOD_COLOR 0x0C // Red
#define BORDER_COLOR 0x0F // White

#define SNAKE_LENGTH 1

extern bool snakes_active;

typedef enum {
    SNAKE_UP,
    SNAKE_DOWN,
    SNAKE_LEFT,
    SNAKE_RIGHT
} Direction;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position position[SNAKE_GAME_WIDTH * SNAKE_GAME_HEIGHT];
    Position food_position;
    int snake_length;
    Direction direction;
    bool game_over;
    int score;
} SnakeGame;

void start_snake_game(void);
void init_snake_game(void);
void update_snake_game(void);
void handle_snake_input(char ascii_char);
void draw_game_board(void);
void draw_snake(void);
void draw_food(void);
void display_score(void);
static void clear_cell(int x, int y);

#endif /* SNAKES_H */