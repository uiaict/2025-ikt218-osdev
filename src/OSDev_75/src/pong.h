#ifndef PONG_H
#define PONG_H

#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "drivers/VGA/vga.h"
#include "drivers/PIT/pit.h"
#include "menu.h"
#include "drivers/audio/song.h"

#define PONG_WIDTH 78
#define PONG_HEIGHT 23
#define PADDLE_HEIGHT 5
#define PADDLE_WIDTH 1
#define BALL_CHAR 'O'
#define PADDLE_CHAR '|'
#define WALL_CHAR '-'
#define EMPTY_CHAR ' '


#define PADDLE_COLOR COLOR8_LIGHT_GREEN
#define BALL_COLOR COLOR8_LIGHT_RED
#define WALL_COLOR COLOR8_LIGHT_BLUE
#define TEXT_COLOR COLOR8_WHITE
#define SCORE_COLOR COLOR8_LIGHT_CYAN
#define BG_COLOR COLOR8_BLACK
#define PAUSE_COLOR COLOR8_YELLOW

#define DIFFICULTY_EASY 1
#define DIFFICULTY_MEDIUM 2
#define DIFFICULTY_HARD 3

typedef struct {
    float x;
    float y;
    float vel_x;
    float vel_y;
} Ball;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t score;
} Paddle;

typedef struct {
    Ball ball;
    Paddle left_paddle;
    Paddle right_paddle;
    bool running;
    uint32_t last_update_time;
    uint8_t difficulty; 
} PongGame;

void init_pong();
void pong_loop();
void update_pong();
void render_pong();
void handle_pong_input();
void reset_ball();
void play_bounce_sound();
void play_score_sound();
float predict_ball_y_position();

extern PongGame pong;
extern uint8_t last_scancode;  

extern float ball_speed_multiplier;
extern float ai_skill_bonus;
extern float ai_prediction_accuracy;

#endif // PONG_H