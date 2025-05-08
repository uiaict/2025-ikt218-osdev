#include "pong.h"
#include "libc/stdio.h"
#include "kernel/system.h"
#include "kernel/pit.h"
#include "kernel/keyboard.h"
#include "apps/shell/shell.h"


int paddle_height = 4;
float paddle_1_y = 12;
float paddle_2_y = 12;
float ball_x = SCREEN_WIDTH / 2;
float ball_y = SCREEN_HEIGHT / 2;
float ball_speed_x = 0.5;
float ball_speed_y = 0.5;

int player_1_score = 0;
int player_2_score = 0;


bool pong_active = false;

void pong_init() {
    pong_active = true;
    for(int i = 0; i < SCREEN_HEIGHT; i++) {
        clear_line(i);
    }
    cursor_disable();

    draw_pong();
}

void draw_paddle(float x, float y) {
    for(int i = 0; i < paddle_height; i++) {
        volatile char *video = cursorPosToAddress(x, y + i);
        *video = '0';
        video++;
        *video = VIDEO_WHITE;
    }
}

void draw_ball(float x, float y) {
    volatile char *video = cursorPosToAddress(x, y);
    *video = 'O';
    video++;
    *video = VIDEO_WHITE;
}

void draw_score(){
    volatile char *video = cursorPosToAddress(SCREEN_WIDTH / 2 - 5, 0);
    *video = 'P';
    video++;
    *video = VIDEO_WHITE;
    video++;
    *video = '1';
    video++;
    *video = VIDEO_WHITE;
    video++;
    *video = ':';
    video++;
    *video = VIDEO_WHITE;
    video++;
    *video = '0' + player_1_score;
    video++;
    *video = VIDEO_WHITE;

    video = cursorPosToAddress(SCREEN_WIDTH / 2 + 1, 0);
    *video = 'P';
    video++;
    *video = VIDEO_WHITE;
    video++;
    *video = '2';
    video++;
    *video = VIDEO_WHITE;
    video++;
    *video = ':';
    video++;
    *video = VIDEO_WHITE;
    video++;
    *video = '0' + player_2_score;
    video++;
    *video = VIDEO_WHITE;
}

void draw_pong() {
    while(pong_active){
        for(int i = 0; i < SCREEN_HEIGHT; i++) {
            clear_line(i);
        }

        // Read input:
        uint8_t sc = inb(0x60);
        if (!(sc & 0x80) && sc < SCANCODE_MAX) {
            char character = scancode_to_ascii[sc];

            if(character == 'w' && paddle_1_y > 0) {
                paddle_1_y--;
            } else if(character == 's' && paddle_1_y < SCREEN_HEIGHT - paddle_height) {
                paddle_1_y++;
            } else if(character == 'i' && paddle_2_y > 0) {
                paddle_2_y--;
            } else if(character == 'k' && paddle_2_y < SCREEN_HEIGHT - paddle_height) {
                paddle_2_y++;
            } else if(character == 'q') {
                pong_active = false;
            }
        }

        draw_paddle(0, paddle_1_y);
        draw_paddle(SCREEN_WIDTH - 1, paddle_2_y);

        draw_ball(ball_x, ball_y);

        ball_x += ball_speed_x;
        ball_y += ball_speed_y;

        if(ball_y <= 0 || ball_y >= SCREEN_HEIGHT - 1) {
            ball_speed_y = -ball_speed_y;
        }

        if(ball_x <= 1 && ball_y >= paddle_1_y && ball_y <= paddle_1_y + paddle_height) {
            ball_speed_x = -ball_speed_x;
        } else if(ball_x >= SCREEN_WIDTH - 2 && ball_y >= paddle_2_y && ball_y <= paddle_2_y + paddle_height) {
            ball_speed_x = -ball_speed_x;
        } else if(ball_x < 0) {
            player_2_score++;
            ball_x = SCREEN_WIDTH / 2;
            ball_y = SCREEN_HEIGHT / 2;
        } else if(ball_x > SCREEN_WIDTH - 1) {
            player_1_score++;
            ball_x = SCREEN_WIDTH / 2;
            ball_y = SCREEN_HEIGHT / 2;
        }

        draw_score();

        sleep_interrupt(33); // 30 FPS
    }

    shell_init();
}