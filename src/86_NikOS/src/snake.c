#include "libc/stdint.h"
#include "libc/string.h"
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "terminal.h"
#include "keyboard.h"
#include "rng.h"
#include "pit.h"
#include "memory.h"

#define SNAKE_UP 0
#define SNAKE_RIGHT 1
#define SNAKE_DOWN 2
#define SNAKE_LEFT 3
#define SNAKE_WAIT 4

#define SNAKE_SPEED 100

#define SNAKE_COLOR get_color(VGA_COLOR_GREEN, VGA_COLOR_GREEN)
#define FOOD_COLOR get_color(VGA_COLOR_RED, VGA_COLOR_DARK_GREY)
#define WALL_COLOR get_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_LIGHT_GREY)
#define FLOOR_COLOR get_color(VGA_COLOR_DARK_GREY, VGA_COLOR_DARK_GREY)

static uint32_t snake_length;
static uint32_t snake_direction;
static uint32_t new_snake_direction;

struct SnakeSegment {
    uint32_t x;
    uint32_t y;
    bool exists;
};

struct Food {
    uint32_t x;
    uint32_t y;
    bool exists;
};

static struct SnakeSegment snake[78 * 20] = {0};
static struct Food food;

static uint32_t score = 0;
static uint32_t timer = 0;
static uint32_t timer_ticks = 0;
static uint32_t timer_seconds = 0;
static uint32_t timer_milliseconds = 0;

static bool updating = false;
static bool has_moved = false;

void food_spawn() {
    struct {
        uint32_t x;
        uint32_t y;
    } possible_positions[78 * 20];
    uint32_t possible_count = 0;

    for (uint32_t y = 0; y < 20; y++) {
        for (uint32_t x = 0; x < 78; x++) {
            bool covered = false;
            for (uint32_t i = 0; i < snake_length; i++) {
                if (snake[i].exists && snake[i].x == x && snake[i].y == y) {
                    covered = true;
                    break;
                }
            }
            if (!covered) {
                possible_positions[possible_count].x = x;
                possible_positions[possible_count].y = y;
                possible_count++;
            }
        }
    }

    if (possible_count > 0) {
        uint32_t random_index = rand_range(0, possible_count - 1);
        food.x = possible_positions[random_index].x;
        food.y = possible_positions[random_index].y;
    }

    food.exists = true;
    terminal_putentryat('@', FOOD_COLOR, food.x + 1, food.y + 4);
}

void snake_init() {
    terminal_clear();
    terminal_disable_cursor();

    // Initial setup of the game area
    for (uint32_t x = 0; x < 80; x++) {
        for (uint32_t y = 3; y < 25; y++) {
            if (x == 0 || x == 79 || y == 3 || y == 24) {
                terminal_putentryat('?', WALL_COLOR, x, y);
            } else {
                terminal_putentryat('L', FLOOR_COLOR, x, y);
            }
        }
    }

    score = 0;
    snake_length = 3;
    snake_direction = SNAKE_WAIT;
    new_snake_direction = SNAKE_WAIT;

    memset(snake, 0, sizeof(snake));

    for (uint32_t i = 0; i < snake_length; i++) {
        snake[i].x = 78 / 2 - i - 2;
        snake[i].y = 20 / 2;
        snake[i].exists = true;
    }

    for (uint32_t i = 0; i < snake_length; i++) {
        if (snake[i].exists) {
            terminal_putentryat('!', SNAKE_COLOR, snake[i].x + 1, snake[i].y + 4);
        }
    }

    food.x = 78 / 2 + 2;
    food.y = 20 / 2;
    food.exists = true;
    terminal_putentryat('@', FOOD_COLOR, food.x + 1, food.y + 4);
}

void snake_change_direction(uint8_t scancode) {
    if (scancode == 0x11) { // 'W'
        if (snake_direction != SNAKE_DOWN) {
            new_snake_direction = SNAKE_UP;
        }
    } else if (scancode == 0x1E) { // 'A'
        if (snake_direction != SNAKE_RIGHT && snake_direction != SNAKE_WAIT) {
            new_snake_direction = SNAKE_LEFT;
        }
    } else if (scancode == 0x1F) { // 'S'
        if (snake_direction != SNAKE_UP) {
            new_snake_direction = SNAKE_DOWN;
        }
    } else if (scancode == 0x20) { // 'D'
        if (snake_direction != SNAKE_LEFT) {
            new_snake_direction = SNAKE_RIGHT;
        }
    }
}

void you_win() {
    not_playing_snake();
    terminal_clear();
    terminal_enable_cursor();
    terminal_writestring_color("YOU WIN!\n", get_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
    terminal_writestring("Score: ");
    terminal_writeint(score);
    terminal_putchar('\n');
    terminal_writestring("> ");
}

void you_lose() {
    not_playing_snake();
    terminal_clear();
    terminal_enable_cursor();
    terminal_writestring_color("GAME OVER!\n", get_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
    terminal_writestring("Score: ");
    terminal_writeint(score);
    terminal_putchar('\n');
    terminal_writestring("> ");
}

void snake_move() {
    if (snake_direction == SNAKE_WAIT && new_snake_direction == SNAKE_WAIT) {
        return;
    }

    snake_direction = new_snake_direction;

    uint32_t snake_head_x = snake[0].x;
    uint32_t snake_head_y = snake[0].y;
    
    switch (snake_direction) {
        case SNAKE_UP:
            snake_head_y--;
            break;
        case SNAKE_RIGHT:
            snake_head_x++;
            break;
        case SNAKE_DOWN:
            snake_head_y++;
            break;
        case SNAKE_LEFT:
            snake_head_x--;
            break;
    }

    terminal_putentryat('!', SNAKE_COLOR, snake_head_x + 1, snake_head_y + 4);

    if (snake_head_x == food.x && snake_head_y == food.y) {
        score++;
        if (snake_length < 78 * 20) {
            snake_length++;
            snake[snake_length - 1].exists = true;
        }
        if (snake_length == 78 * 20) {
            you_win();
            return;
        } else {
            food.exists = false;
        }
    }

    for (uint32_t i = snake_length - 1; i > 0; i--) {
        if (i == snake_length - 1) {
            terminal_putentryat('L', FLOOR_COLOR, snake[i].x + 1, snake[i].y + 4);
        }
        if (snake[i].exists) {
            snake[i].x = snake[i - 1].x;
            snake[i].y = snake[i - 1].y;
        }
    }

    snake[0].x = snake_head_x;
    snake[0].y = snake_head_y;

    if (snake[0].x >= 78 || snake[0].y >= 20) {
        you_lose();
        return;
    }
    for (uint32_t i = 1; i < snake_length; i++) {
        if (snake[i].x == snake[0].x && snake[i].y == snake[0].y && snake[i].exists) {
            you_lose();
            return;
        }
    }

    if (!food.exists) {
        food_spawn();
    }

    has_moved = true;
}

void snake_draw() {
    terminal_setcursor(78 / 4 - 5, 1);
    terminal_writestring_color("Score: ", get_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    terminal_writeint_color(score, get_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    terminal_setcursor(78 * 3 / 4 - 4, 1);
    terminal_writestring_color("Time: ", get_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    terminal_writeint_color(timer_seconds, get_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    terminal_writestring_color("s", get_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    terminal_setcursor(0, 0);
}

void snake_update() {
    if (updating) {
        return;
    }
    updating = true;
    timer++;
    if (has_moved) timer_ticks++;
    timer_milliseconds = timer_ticks / TICKS_PER_MS;
    timer_seconds = timer_milliseconds / 1000;
    if (timer / TICKS_PER_MS >= SNAKE_SPEED) {
        timer = 0;
        snake_move();
        if (is_playing_snake()) {
            snake_draw();
        }
    }
    updating = false;
}