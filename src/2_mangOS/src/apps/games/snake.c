#include "libc/stdio.h"
#include "libc/stdlib.h"
#include "libc/terminal.h"
#include "libc/stdbool.h"
#include "libc/rand.h"
#include "games/snake.h"
#include "keyboard.h"
#include "pit.h"

#define MAX_SNAKE_LEN 1024
#define TICKS_PER_FRAME 100

typedef struct
{
    int x, y;
} Point;

enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction;

Point snake[MAX_SNAKE_LEN];
int snake_length;

Point food;
int score;
bool game_over;
extern uint32_t ticks;

bool position_on_snake(Point food)
{
    for (int i = 0; i < snake_length; i++)
    {
        if (snake[i].x == food.x && snake[i].y == food.y)
        {
            return true; // Collision detected
        }
    }
    return false; // No collision
}

void place_food()
{
    do
    {
        food.x = rand() % 78 + 1;
        food.y = rand() % 23 + 1;
    } while (position_on_snake(food));
}

bool check_self_collision(Point head)
{
    for (int i = 1; i < snake_length; i++)
        if (snake[i].x == head.x && snake[i].y == head.y)
            return true;
    return false;
}

void init_snake()
{
    snake_length = 3;
    snake[0] = (Point){40, 12}; // head
    snake[1] = (Point){39, 12};
    snake[2] = (Point){38, 12};
    direction = RIGHT;

    // Place first food
    food.x = rand() % 78 + 1;
    food.y = rand() % 23 + 1;
}

void handle_input(void)
{
    char c = peekChar();
    if (!c)
        return; // nothing pressed this tick

    getChar();

    // map to directions
    if (c == 'w' && direction != DOWN)
        direction = UP;
    else if (c == 's' && direction != UP)
        direction = DOWN;
    else if (c == 'a' && direction != RIGHT)
        direction = LEFT;
    else if (c == 'd' && direction != LEFT)
        direction = RIGHT;
}

void draw_frame()
{
    terminal_clear();

    terminal_setcolor(VGA_COLOR_YELLOW);
    printf("Score: ");
    terminal_write_dec(score);

    uint8_t color = VGA_COLOR_GREEN;
    // Draw food
    terminal_putentryat('*', color, food.x, food.y);

    // Draw snake
    for (int i = 0; i < snake_length; i++)
    {
        char glyph = (i == 0) ? 'O' : 'o';
        terminal_putentryat(glyph, color, snake[i].x, snake[i].y);
    }
    move_cursor_to(0, 24);
}

void update_snake()
{
    // Compute new head
    Point head = snake[0];
    switch (direction)
    {
    case UP:
        head.y--;
        break;
    case DOWN:
        head.y++;
        break;
    case LEFT:
        head.x--;
        break;
    case RIGHT:
        head.x++;
        break;
    }

    // Check wall or self-collision → game_over = true;
    if (head.x <= 0 || head.x >= 79 || head.y <= 0 || head.y >= 24 || check_self_collision(head))
    {
        game_over = true;
        return;
    }

    // Insert new head
    // Shift all segments up by one
    for (int i = snake_length; i > 0; i--)
    {
        snake[i] = snake[i - 1];
    }
    snake[0] = head;

    // Check food
    if (head.x == food.x && head.y == food.y)
    {
        snake_length++;
        score++;
        // Place new food somewhere not on the snake
        place_food();
    }
    else
    {
        // Remove last segment by truncating length (we already shifted)
        // snake_length stays the same, so tail drops off
    }
}

void start_snake_game()
{
    // Initialize game state
    clearBuffer();
    terminal_clear();
    init_snake();
    score = 0;
    game_over = 0;
    while (!game_over)
    {
        // Wait for next tick (e.g. every 100ms)
        uint32_t target = ticks + TICKS_PER_FRAME;
        while (ticks < target)
        {
            handle_input();
            asm volatile("hlt");
        }

        update_snake();
        draw_frame();
    }
    return;
}