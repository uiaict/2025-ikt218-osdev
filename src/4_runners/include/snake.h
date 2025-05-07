#ifndef SNAKE_H
#define SNAKE_H

#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "irq.h"  // Include for scan codes and game mode functions

/* Game constants */
#define SNAKE_BOARD_WIDTH  40
#define SNAKE_BOARD_HEIGHT 20
#define SNAKE_START_X      10
#define SNAKE_START_Y      10
#define SNAKE_TICK_MS     50  /* Base tick rate in milliseconds */


/* Game direction */
typedef enum {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
} Direction;

/* Game state */
typedef enum {
    GAME_STATE_RUNNING,
    GAME_STATE_PAUSED,
    GAME_STATE_OVER
} GameState;

/* Game interface */
typedef struct {
    void     (*init)(void);
    void     (*update)(void);
    void     (*handle_input)(uint8_t sc);
    uint32_t (*get_score)(void);
    GameState (*get_state)(void);
} SnakeGame;

/* Functions exported to the OS */
SnakeGame* create_snake_game(void);
void snake_tick(void);
void snake_on_key(uint8_t sc);

#endif /* SNAKE_H */