// games/snake.h
#ifndef SNAKE_H
#define SNAKE_H

#include <libc/stdint.h>
#include <libc/stdbool.h>

#define BOARD_WIDTH    80
#define BOARD_HEIGHT   25
#define SNAKE_MAX_LEN ((BOARD_WIDTH * BOARD_HEIGHT) / 2)

typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } Direction;

// Call this after all inits; never returns
void snake_run(void);

#endif // SNAKE_H
