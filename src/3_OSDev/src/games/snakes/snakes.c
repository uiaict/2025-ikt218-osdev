#include <libc/stdbool.h>
#include <libc/stdint.h>
#include <libc/stdio.h>
#include "games/snakes/snakes.h"
#include "menu.h"
#include "utils.h"
#include "pit.h"
#include "vga.h"
#include "interrupts.h"
#include "memory/memory.h"

SnakeGame snake_game;

void allocate_snake_memory(int initial_size) {
    snake_game.position = (Position*)malloc(initial_size * sizeof(Position));
    snake_game.capacity = initial_size;
    
    // Initialize positions to invalid coordinates
    for (int i = 0; i < initial_size; i++) {
        snake_game.position[i].x = -1;
        snake_game.position[i].y = -1;
    }
}

void grow_snake_memory(int new_capacity) {
    Position* new_position = (Position*)malloc(new_capacity * sizeof(Position));
    
    // Copy existing positions to new array
    for (int i = 0; i < snake_game.snake_length; i++) {
        new_position[i] = snake_game.position[i];
    }
    
    // Initialize new positions to invalid coordinates
    for (int i = snake_game.snake_length; i < new_capacity; i++) {
        new_position[i].x = -1;
        new_position[i].y = -1;
    }
    
    // Free old memory and update pointers
    free(snake_game.position);
    snake_game.position = new_position;
    snake_game.capacity = new_capacity;
}

void free_snake_memory(void) {
    if (snake_game.position != NULL) {
        free(snake_game.position);
        snake_game.position = NULL;
        snake_game.capacity = 0;
    }
}

void start_snake_game(void) {
    // Initialize the game
    init_snake_game();
    
    // Main game loop
    enable_interrupts();
    while (!snake_game.game_over) {
        update_snake_game();
        draw_snake();
        enable_interrupts();
        sleep_interrupt(100); 
    }

    // Free snake memory
    free_snake_memory();

    // Game over message
    set_cursor_position(SNAKE_GAME_WIDTH/2 - 5, SNAKE_GAME_HEIGHT/2);
    printf(0x0F, "GAME OVER!");
    sleep_busy(2000);

    display_menu();
}

void init_snake_game(void) {
    snakes_active = true;
    // Set random seed
    srand(get_current_ticks()); 
    
    // Allocate memory for snake positions
    allocate_snake_memory(INITIAL_CAPACITY);
    
    // Initialize game variables, snake position, food position, etc.
    snake_game.snake_length = SNAKE_LENGTH;
    snake_game.direction = SNAKE_RIGHT;
    snake_game.game_over = false;
    snake_game.score = 0;
    snake_game.food_position.x = -1;
    snake_game.food_position.y = -1;
    
    draw_game_board();
    
    // Place the snake in the middle of the game board
    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snake_game.position[i].x = SNAKE_GAME_WIDTH / 2 - i;
        snake_game.position[i].y = SNAKE_GAME_HEIGHT / 2;
    }
}

void update_snake_game(void) {
    // Remove the last segment of the snake
    Position old_tail = snake_game.position[snake_game.snake_length - 1];
    clear_cell(old_tail.x, old_tail.y);

    // Shift the body
    for (int i = snake_game.snake_length - 1; i > 0; i--) {
        snake_game.position[i] = snake_game.position[i - 1];
    }
    
    // Move the head
    Position new_head = snake_game.position[0];
    switch (snake_game.direction) {
        case SNAKE_UP:
            new_head.y--;
            break;
        case SNAKE_DOWN:
            new_head.y++;
            break;
        case SNAKE_LEFT:
            new_head.x--;
            break;
        case SNAKE_RIGHT:
            new_head.x++;
            break;
    }

    if (new_head.x <0 || new_head.x >= SNAKE_GAME_WIDTH || new_head.y < 0 || new_head.y >= SNAKE_GAME_HEIGHT) {
        snake_game.game_over = true;
        return;
    }
    for (int i = 0; i < snake_game.snake_length; i++) {
        if (snake_game.position[i].x == new_head.x && snake_game.position[i].y == new_head.y) {
            snake_game.game_over = true;
            return;
        }
    }

    snake_game.position[0] = new_head;

    // Check for food collision
    if (new_head.x == snake_game.food_position.x && new_head.y == snake_game.food_position.y) {
        snake_game.score += 10;

        // Check if we need to grow the memory allocation
        if (snake_game.snake_length + 1 >= snake_game.capacity) {
            grow_snake_memory(snake_game.capacity * 2);
        }

        snake_game.snake_length++;
        snake_game.position[snake_game.snake_length - 1] = old_tail;

        // Generate new food
        snake_game.food_position.x = -1;
        snake_game.food_position.y = -1;
    }

    // Redraw food and score
    draw_snake();
    draw_food();
    display_score();
}

void handle_snake_input(char ascii_char) {
    if (ascii_char == 'w' && snake_game.direction != SNAKE_DOWN) {
        snake_game.direction = SNAKE_UP;
    } else if (ascii_char == 's' && snake_game.direction != SNAKE_UP) {
        snake_game.direction = SNAKE_DOWN;
    } else if (ascii_char == 'a' && snake_game.direction != SNAKE_RIGHT) {
        snake_game.direction = SNAKE_LEFT;
    } else if (ascii_char == 'd' && snake_game.direction != SNAKE_LEFT) {
        snake_game.direction = SNAKE_RIGHT;
    }
}

void draw_game_board(void) {
    int x, y;

    /* Top border */
    for (x = 0; x < SNAKE_GAME_WIDTH + 2; x++) {
        printf(BORDER_COLOR, "#");
    }
    printf(BORDER_COLOR, "\n");

    /* Middle rows */
    for (y = 2; y < SNAKE_GAME_HEIGHT+2; y++) {
        /* Left wall */
        printf(BORDER_COLOR, "#");
        /* Interior */
        for (x = 0; x < SNAKE_GAME_WIDTH; x++) {
            printf(BORDER_COLOR, " ");
        }
        /* Right wall + newline */
        printf(BORDER_COLOR, "#\n");
    }

    /* Bottom border */
    for (x = 0; x < SNAKE_GAME_WIDTH + 2; x++) {
        printf(BORDER_COLOR, "#");
    }
    printf(BORDER_COLOR, "\n");
}

void draw_snake(void) {
    for (int i = 0; i < snake_game.snake_length; i++) {
        set_cursor_position(
            snake_game.position[i].x + 1,
            snake_game.position[i].y + 1
        );
        printf(SNAKE_COLOR, "O");
    }
    set_cursor_position(-1, -1);
}

void draw_food(void) {
    // Static food position 
    static Position food = {-1, -1};

    // Only generate a new food if one doesn't exist
    if (snake_game.food_position.x == -1 && snake_game.food_position.y == -1) {
        bool valid_position = false;

        while (!valid_position) {
            food.x = rand() % SNAKE_GAME_WIDTH;
            food.y = rand() % SNAKE_GAME_HEIGHT;
            valid_position = true;
            for (int i = 0; i < snake_game.snake_length; i++) {
                if (snake_game.position[i].x == food.x && snake_game.position[i].y == food.y) {
                    valid_position = false;
                    break;
                }
            }
        }
    }
    set_cursor_position(food.x + 1, food.y + 1);
    printf(FOOD_COLOR, "O");
    set_cursor_position(-1,-1);
    snake_game.food_position = food;
}

static void clear_cell(int x, int y) {
    set_cursor_position(x + 1, y + 1);
    printf(BORDER_COLOR, " ");
    set_cursor_position(-1,-1);
}


void display_score(void) {
    // Display the current score on the screen
    set_cursor_position(0, SNAKE_GAME_HEIGHT + 3);
    printf(0x0F, "Score: %d\n", snake_game.score);
    set_cursor_position(-1,-1);
}

