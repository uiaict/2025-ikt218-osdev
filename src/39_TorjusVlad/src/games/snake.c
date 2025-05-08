#include "arch/i386/console.h"
#include "keyboard.h"
#include "games/snake.h"
#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "libc/portio.h"
#include "libc/random.h"
#include "pit.h"

#define WIDTH 80
#define HEIGHT 25
#define VIDEO_MEMORY ((uint16_t*)0xB8000)
#define COLOR 0x0F

enum Direction { UP, DOWN, LEFT, RIGHT };

typedef struct {
    int x, y;
} Vec2;

Vec2 snake[100];
int snake_length = 5;
enum Direction dir = RIGHT;
Vec2 food = {10, 10};

static void draw_char(int x, int y, char c) {
    VIDEO_MEMORY[y * WIDTH + x] = (COLOR << 8) | c;
}

static void clear_screen() {
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            draw_char(x, y, ' ');
}

static void draw_snake() {
    for (int i = 0; i < snake_length; i++)
        draw_char(snake[i].x, snake[i].y, i == 0 ? '@' : 'o');
}

static void draw_food() {
    draw_char(food.x, food.y, '*');
}

static void draw_game_over() {
    const char* msg = "GAME OVER";
    const char* prompt = "Press any key to exit...";
    int len = 9;
    int x = (WIDTH - len) / 2;
    int y = HEIGHT / 2;
    for (int i = 0; i < len; i++)
        draw_char(x + i, y, msg[i]);

    int prompt_x = (WIDTH - 24) / 2;
    for (int i = 0; prompt[i]; i++)
        draw_char(prompt_x + i, y + 2, prompt[i]);
}

static void draw_score(int score) {
    char buffer[16];
    int i = 0;

    if (score == 0) {
        buffer[i++] = '0';
    } else {
        while (score > 0 && i < sizeof(buffer) - 1) {
            buffer[i++] = '0' + (score % 10);
            score /= 10;
        }
    }

    buffer[i] = '\0';

    // Reverse string
    for (int j = 0; j < i / 2; j++) {
        char tmp = buffer[j];
        buffer[j] = buffer[i - 1 - j];
        buffer[i - 1 - j] = tmp;
    }

    const char* label = "Score: ";
    for (int j = 0; label[j]; j++)
        draw_char(j, 0, label[j]);
    for (int j = 0; buffer[j]; j++)
        draw_char(7 + j, 0, buffer[j]);
}

static void draw_intro_screen() {
    clear_screen();

    const char* title = "SNAKE GAME";
    const char* controls = "Controls: W = up, A = left, S = down, D = right, Q = quit";
    const char* prompt = "Press Enter to start...";

    int title_x = (WIDTH - 10) / 2;
    int controls_x = (WIDTH - 52) / 2;
    int prompt_x = (WIDTH - 24) / 2;

    int y = HEIGHT / 2 - 2;

    for (int i = 0; title[i]; i++)
        draw_char(title_x + i, y, title[i]);

    y += 2;
    for (int i = 0; controls[i]; i++)
        draw_char(controls_x + i, y, controls[i]);

    y += 2;
    for (int i = 0; prompt[i]; i++)
        draw_char(prompt_x + i, y, prompt[i]);
}

static bool check_self_collision() {
    for (int i = 1; i < snake_length; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            return true;
    }
    return false;
}

static void move_snake() {
    for (int i = snake_length - 1; i > 0; i--)
        snake[i] = snake[i - 1];

    switch (dir) {
        case UP:    snake[0].y--; break;
        case DOWN:  snake[0].y++; break;
        case LEFT:  snake[0].x--; break;
        case RIGHT: snake[0].x++; break;
    }

    // Wrap around screen edges
    if (snake[0].x < 0) snake[0].x = WIDTH - 1;
    if (snake[0].x >= WIDTH) snake[0].x = 0;
    if (snake[0].y < 0) snake[0].y = HEIGHT - 1;
    if (snake[0].y >= HEIGHT) snake[0].y = 0;

    // Eat food
    if (snake[0].x == food.x && snake[0].y == food.y) {
        if (snake_length < 100) snake_length++;
        food.x = (int)(random() * WIDTH);
        food.y = (int)(random() * (HEIGHT - 1));
    }
}

static bool handle_input() {
    char c = keyboard_get_char(); 
    if (c == 'w' && dir != DOWN) dir = UP;
    if (c == 's' && dir != UP) dir = DOWN;
    if (c == 'a' && dir != RIGHT) dir = LEFT;
    if (c == 'd' && dir != LEFT) dir = RIGHT;
    if (c == 'q') return true; // Exit game
    return false;
}

void snake_main() {
    draw_intro_screen();

    // Wait for Enter key
    while (1) {
        char c = keyboard_get_char();
        if (c == '\r' || c == '\n') break;
    }

    clear_screen();

    for (int i = 0; i < snake_length; i++)
        snake[i] = (Vec2){10 - i, 10};

    while (1) {
        clear_screen();
        if (handle_input()) break;
        move_snake();
        
        if (check_self_collision()) {
            clear_screen();
            draw_game_over();
            break;
        }

        draw_snake();
        draw_food();
        draw_score(snake_length - 5);
        sleep_interrupt(50);
    }

    while (!keyboard_get_char()) { console_clear(); }
}
