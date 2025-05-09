#include "snake.h"
#include "keyboard.h"
#include "terminal.h"
#include "common.h"
#include "system.h"
#include <stdint.h>
#include "pit.h"

#define WIDTH 80  // Grid width
#define HEIGHT 22 // Grid height

// Directions: up, right, down, left
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define FOOD_MAX_Y (HEIGHT - 3) // Avoid bottom 2 rows (used for text/UI)

static uint32_t seed = 444;

// Snake structure
typedef struct {
    int x, y;
} Position;

Position snake[100]; // Snake body
int snake_length = 5;  // Starting length of the snake
int dir = RIGHT;  // Initial direction of the snake
int food_x, food_y;  // Food coordinates
int game_active = 1;
Position free_positions[WIDTH * HEIGHT];

int rand() {
    seed = seed * 1103515245 + 12345;
    return (seed / 65536) % 32768;
}

int is_snake_position(int x, int y, int skip_index) {
    for (int i = 0; i < snake_length; i++) {
        if (i == skip_index) continue; // Skip the segment we don't want to check
        if (snake[i].x == x && snake[i].y == y) {
            return 1; // Occupied
        }
    }
    return 0; // Free
}

void draw_border() {
    // Top and bottom border
    for (int x = 0; x < WIDTH; x++) {
        terminal_putchar_at('#', x, 0);
        terminal_putchar_at('#', x, HEIGHT - 1);
    }

    // Left and right border
    for (int y = 0; y < HEIGHT; y++) {
        terminal_putchar_at('#', 0, y);
        terminal_putchar_at('#', WIDTH - 1, y);
    }
}

void place_food() {
    int count = 0;

    // Build list of all free tiles
    for (int y = 0; y <= FOOD_MAX_Y; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (!is_snake_position(x, y, -1)) {
                free_positions[count].x = x;
                free_positions[count].y = y;
                count++;
            }
        }
    }

    if (count == 0) {
        // No space left — player wins!
        printf("You win!\n");
        sleep_interrupt(1000);
        return;
    }

    // Pick one of the free spots randomly
    int i = rand() % count;
    food_x = free_positions[i].x;
    food_y = free_positions[i].y;

    terminal_putchar_at('@', food_x, food_y);
}

void init_snake() {
    // Clear the terminal
    terminal_initialize();
    
    // Initialize snake's starting position (middle of the screen)
    for (int i = 0; i < snake_length; i++) {
        snake[i].x = WIDTH / 2 - i;
        snake[i].y = HEIGHT / 2;
    }

    // Place food at random position (for simplicity)
    food_x = rand() % WIDTH;
    food_y = rand() % HEIGHT;
    
    draw_border();
}

void clear_tail() {
    Position tail = snake[snake_length - 1];
    terminal_putchar_at(' ', tail.x, tail.y);  // Overwrite tail with space
}

void render_snake() {
    clear_tail();

    // Draw snake
    for (int i = 0; i < snake_length; i++) {
        terminal_putchar_at('*', snake[i].x, snake[i].y);
    }

    // Draw food
    terminal_putchar_at('@', food_x, food_y);

    // Display score and controls at fixed location (bottom of screen)
    int status_y = HEIGHT + 1;
    const char* msg1 = "Snake Length: ";
    const char* msg2 = "Use WASD to control.";

    for (int i = 0; msg1[i]; i++) {
        terminal_putchar_at(msg1[i], i, status_y);
    }

    // Print snake length value manually
    char score_str[4];
    int_to_string(snake_length, score_str);
    for (int i = 0; score_str[i]; i++) {
        terminal_putchar_at(score_str[i], i + 14, status_y); // 14 = length of msg1
    }

    for (int i = 0; msg2[i]; i++) {
        terminal_putchar_at(msg2[i], i, status_y + 1);
    }
}

void update_snake() {
    // Save current tail before moving
    Position old_tail = snake[snake_length - 1];

    // Compute new head
    Position new_head = snake[0];
    if (dir == UP) new_head.y--;
    else if (dir == DOWN) new_head.y++;
    else if (dir == LEFT) new_head.x--;
    else if (dir == RIGHT) new_head.x++;

    if (new_head.x < 0 || new_head.x >= WIDTH - 1||
        new_head.y < 0 || new_head.y >= HEIGHT -1 ||  // Avoid UI area
        is_snake_position(new_head.x, new_head.y, 0)) {  // Skip checking against head itself
        printf("Game Over!\n");
        sleep_interrupt(2000);
        terminal_initialize();
        game_active = 0;
        return;
    }    

    // Shift body forward
    for (int i = snake_length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = new_head;

    // Check for food
    if (new_head.x == food_x && new_head.y == food_y) {
        // Place new food
        place_food();

        // Grow snake: copy last segment again
        snake[snake_length] = old_tail;
        snake_length++;
    } else {
        // Erase old tail
        terminal_putchar_at(' ', old_tail.x, old_tail.y);
    }

    // Draw head
    terminal_putchar_at('*', new_head.x, new_head.y);
}



void game_loop() {
    while (game_active) {
        render_snake();

        static char last_input = 0;
        char input = keyboard_getchar_nb();
        
        // Only act if the key is different from last frame
        if (input != 0 && input != last_input) {
            if (input == 'w' && dir != DOWN) dir = UP;
            else if (input == 's' && dir != UP) dir = DOWN;
            else if (input == 'a' && dir != RIGHT) dir = LEFT;
            else if (input == 'd' && dir != LEFT) dir = RIGHT;
        
            last_input = input;
        } else if (input == 0) {
            last_input = 0; // Reset so new input can be read next frame
        }

        update_snake();

        // Delay loop to control the speed of the game
        sleep_busy(100);
    }
}

void play_snake() {
    game_active = 1;
    init_snake();
    game_loop();
}