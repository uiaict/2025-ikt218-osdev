#ifndef SNAKE_H
#define SNAKE_H

#include "libc/stdint.h"

#define SNAKE_BOARD_WIDTH  15  // Width of the snake game board (in tiles)
#define SNAKE_BOARD_HEIGHT 17 // Height of the snake game board (in tiles) 
#define SNAKE_MAX_LENGTH   64 // Maximum number of segments the snake can grow to
#define VGA_WIDTH  80 // Width of the VGA text screen (in characters)
#define VGA_HEIGHT 25 // Height of the VGA text screen (in characters)
#define VGA_MEMORY ((volatile char*) 0xB8000) // VGA memory address
#define OFFSET_X ((VGA_WIDTH - SNAKE_BOARD_WIDTH) / 2) // Center the board horizontally
#define OFFSET_Y ((VGA_HEIGHT - SNAKE_BOARD_HEIGHT) / 2) // Center the board vertically
#define TILE_IDX(x, y)  (((y)-1) * (SNAKE_BOARD_WIDTH - 2) + ((x)-1)) // 1D index for 2D coordinates
#define SCORE_ROW   (OFFSET_Y + SNAKE_BOARD_HEIGHT) // Row for score display

// colors
#define CLR_GREEN 0x0A   // green (boarder)
#define CLR_RED   0x0C   // red (game over)
#define CLR_NORMAL 0x0F   // white (snake)
#define CLR_YELLOW 0x0E   // yellow (food)  

// Enum for board cell types
typedef enum {
    TILE_EMPTY,
    TILE_SNAKE,
    TILE_FOOD,
    TILE_BORDER
} TileType;

// Point type for position tracking
typedef struct {
    int x;
    int y;
} Point;

// Snake representation
typedef struct {
    Point segments[SNAKE_MAX_LENGTH];
    int length;
    int direction; // 0=UP, 1=DOWN, 2=LEFT, 3=RIGHT
} Snake;

// Full game state
typedef struct {
    TileType tiles[SNAKE_BOARD_HEIGHT][SNAKE_BOARD_WIDTH];
    Snake snake;
    Point food;
    int game_over;
} SnakeGame;

// Entry point for the game

void game_draw_title(void); 
void game_clear_screen();
void game_draw_char(int x, int y, char c, uint8_t color);
void game_draw_string(int x, int y, const char* str, uint8_t color);
void snake_main(void);

#endif