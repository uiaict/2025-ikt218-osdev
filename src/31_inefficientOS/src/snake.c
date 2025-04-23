#include "snake.h"
#include "terminal.h"
#include "keyboard.h"
#include "memory.h"
#include "common.h"
#include "menu.h" // For key definitions

// Define game area boundaries
#define GAME_WIDTH 40
#define GAME_HEIGHT 20
#define INITIAL_SNAKE_LENGTH 3
#define MAX_SNAKE_LENGTH 100

// Define the VGA width if not already defined
#ifndef VGA_WIDTH
#define VGA_WIDTH 80
#endif

// Declaration for the sleep_busy function from common.c
extern void sleep_busy(uint32_t ms);
extern void sleep_interrupt(uint32_t ms);

// Game states
typedef enum {
    GAME_RUNNING,
    GAME_OVER,
    GAME_EXIT
} GameState;

// Direction enum
typedef enum {
    DIR_UP,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT
} Direction;

// Snake segment structure
typedef struct {
    int x;
    int y;
} SnakeSegment;

// Game structure
typedef struct {
    SnakeSegment snake[MAX_SNAKE_LENGTH];
    int length;
    Direction direction;
    int food_x;
    int food_y;
    GameState state;
    int score;
    int speed;
} SnakeGame;

// Function prototypes
static void snake_init(SnakeGame* game);
static void snake_place_food(SnakeGame* game);
static void snake_update(SnakeGame* game);
static void snake_draw(SnakeGame* game);
static void snake_handle_input(SnakeGame* game, uint8_t scancode);
static void draw_border();
static void draw_game_over(int score);

// Helper function for string conversion
static void snake_int_to_str(int num, char* str);

// Start the Snake game
void snake_game_start() {
    terminal_initialize();
    terminal_write_colored("===== Snake Game =====\n\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("Use arrow keys to control the snake.\n");
    terminal_writestring("Eat food (X) to grow and earn points.\n");
    terminal_writestring("Avoid hitting the walls or yourself.\n");
    terminal_writestring("Press ESC to exit the game.\n\n");
    terminal_writestring("Press any key to start...");
    
    // Wait for a key press to start
    uint8_t scancode;
    while (1) {
        scancode = keyboard_get_scancode();
        if (scancode != 0) {
            break;
        }
        sleep_busy(10);
    }
    
    // Initialize game
    SnakeGame game;
    snake_init(&game);
    
    // Game loop
    while (game.state != GAME_EXIT) {
        // Check for input
        scancode = keyboard_get_scancode();
        if (scancode != 0) {
            snake_handle_input(&game, scancode);
        }
        
        if (game.state == GAME_RUNNING) {
            // Update game state
            snake_update(&game);
            
            // Draw game
            snake_draw(&game);
            
            // Delay based on game speed
            sleep_busy(200 - (game.speed * 5));
        } else if (game.state == GAME_OVER) {
            // Display game over screen
            draw_game_over(game.score);
            
            // Wait for key press to restart or exit
            bool restart = false;
            while (!restart && game.state != GAME_EXIT) {
                scancode = keyboard_get_scancode();
                if (scancode != 0) {
                    if (scancode == KEY_ESC) {
                        game.state = GAME_EXIT;
                    } else if (scancode == KEY_ENTER) {
                        restart = true;
                        snake_init(&game);
                    }
                }
                sleep_busy(10);
            }
        }
    }
    
    // Return to main menu
    terminal_initialize();
}

// Initialize the snake game
static void snake_init(SnakeGame* game) {
    // Set initial snake position (center of game area)
    int center_x = GAME_WIDTH / 2;
    int center_y = GAME_HEIGHT / 2;
    
    // Initialize snake
    game->length = INITIAL_SNAKE_LENGTH;
    for (int i = 0; i < game->length; i++) {
        game->snake[i].x = center_x - i;
        game->snake[i].y = center_y;
    }
    
    // Set initial direction
    game->direction = DIR_RIGHT;
    
    // Initialize game state
    game->state = GAME_RUNNING;
    game->score = 0;
    game->speed = 1;
    
    // Place initial food
    snake_place_food(game);
    
    // Draw initial state
    terminal_initialize();
    draw_border();
    snake_draw(game);
}

// Place food at a random position
static void snake_place_food(SnakeGame* game) {
    // Simple random number generation using a static seed
    static uint32_t seed = 12345;
    
    // Generate a new position for food
    bool valid_position = false;
    int x, y;
    
    while (!valid_position) {
        // Simple PRNG
        seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
        x = (seed % (GAME_WIDTH - 2)) + 1;
        
        seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
        y = (seed % (GAME_HEIGHT - 2)) + 1;
        
        // Check if position is valid (not occupied by snake)
        valid_position = true;
        for (int i = 0; i < game->length; i++) {
            if (game->snake[i].x == x && game->snake[i].y == y) {
                valid_position = false;
                break;
            }
        }
    }
    
    // Set food position
    game->food_x = x;
    game->food_y = y;
}

// Update the snake game state
static void snake_update(SnakeGame* game) {
    // Store the current head position
    int head_x = game->snake[0].x;
    int head_y = game->snake[0].y;
    
    // Calculate new head position based on direction
    switch (game->direction) {
        case DIR_UP:
            head_y--;
            break;
        case DIR_RIGHT:
            head_x++;
            break;
        case DIR_DOWN:
            head_y++;
            break;
        case DIR_LEFT:
            head_x--;
            break;
    }
    
    // Check for collisions
    if (head_x <= 0 || head_x >= GAME_WIDTH - 1 || 
        head_y <= 0 || head_y >= GAME_HEIGHT - 1) {
        // Hit the wall
        game->state = GAME_OVER;
        return;
    }
    
    // Check if snake hits itself
    for (int i = 1; i < game->length; i++) {
        if (head_x == game->snake[i].x && head_y == game->snake[i].y) {
            game->state = GAME_OVER;
            return;
        }
    }
    
    // Check if snake eats food
    bool ate_food = (head_x == game->food_x && head_y == game->food_y);
    
    // Move snake body (don't move the last segment if food was eaten)
    int move_length = ate_food ? game->length : game->length - 1;
    for (int i = move_length; i > 0; i--) {
        game->snake[i].x = game->snake[i-1].x;
        game->snake[i].y = game->snake[i-1].y;
    }
    
    // Update head position
    game->snake[0].x = head_x;
    game->snake[0].y = head_y;
    
    // Handle food consumption
    if (ate_food) {
        // Increase score
        game->score += 10;
        
        // Increase length
        game->length++;
        
        // Increase speed every 5 food items
        if (game->score % 50 == 0 && game->speed < 20) {
            game->speed++;
        }
        
        // Place new food
        snake_place_food(game);
    }
}

// Draw the snake game
static void snake_draw(SnakeGame* game) {
    // Clear the game area (keep the border)
    for (int y = 1; y < GAME_HEIGHT - 1; y++) {
        for (int x = 1; x < GAME_WIDTH - 1; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = vga_entry(' ', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
        }
    }
    
    // Draw food
    terminal_buffer[game->food_y * VGA_WIDTH + game->food_x] = vga_entry('X', vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    
    // Draw snake
    for (int i = 0; i < game->length; i++) {
        // Use different colors for head and body
        enum vga_color color = (i == 0) ? VGA_COLOR_LIGHT_GREEN : VGA_COLOR_GREEN;
        char symbol = (i == 0) ? 'O' : 'o';
        
        terminal_buffer[game->snake[i].y * VGA_WIDTH + game->snake[i].x] = 
            vga_entry(symbol, vga_entry_color(color, VGA_COLOR_BLACK));
    }
    
    // Draw score
    char score_text[20] = "Score: ";
    char score_str[10];
    snake_int_to_str(game->score, score_str);
    
    int i = 7; // Start after "Score: "
    int j = 0;
    while (score_str[j] != '\0') {
        score_text[i++] = score_str[j++];
    }
    score_text[i] = '\0';
    
    for (i = 0; score_text[i] != '\0'; i++) {
        terminal_buffer[(GAME_HEIGHT + 1) * VGA_WIDTH + i + 1] = 
            vga_entry(score_text[i], vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    }
}

// Handle input for the snake game
static void snake_handle_input(SnakeGame* game, uint8_t scancode) {
    switch (scancode) {
        case KEY_UP:
            if (game->direction != DIR_DOWN) {
                game->direction = DIR_UP;
            }
            break;
        case KEY_RIGHT:
            if (game->direction != DIR_LEFT) {
                game->direction = DIR_RIGHT;
            }
            break;
        case KEY_DOWN:
            if (game->direction != DIR_UP) {
                game->direction = DIR_DOWN;
            }
            break;
        case KEY_LEFT:
            if (game->direction != DIR_RIGHT) {
                game->direction = DIR_LEFT;
            }
            break;
        case KEY_ESC:
            game->state = GAME_EXIT;
            break;
    }
}

// Draw the game border
static void draw_border() {
    // Draw horizontal borders
    for (int x = 0; x < GAME_WIDTH; x++) {
        // Top border
        terminal_buffer[0 * VGA_WIDTH + x] = 
            vga_entry('#', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
        
        // Bottom border
        terminal_buffer[(GAME_HEIGHT - 1) * VGA_WIDTH + x] = 
            vga_entry('#', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    }
    
    // Draw vertical borders
    for (int y = 0; y < GAME_HEIGHT; y++) {
        // Left border
        terminal_buffer[y * VGA_WIDTH + 0] = 
            vga_entry('#', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
        
        // Right border
        terminal_buffer[y * VGA_WIDTH + (GAME_WIDTH - 1)] = 
            vga_entry('#', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    }
}

// Draw game over screen
static void draw_game_over(int score) {
    // Clear the center of the screen
    for (int y = 5; y < 15; y++) {
        for (int x = 10; x < 70; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = vga_entry(' ', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
        }
    }
    
    // Game over text
    const char* game_over = "GAME OVER";
    int start_x = (GAME_WIDTH - 9) / 2;
    
    for (int i = 0; game_over[i] != '\0'; i++) {
        terminal_buffer[7 * VGA_WIDTH + (start_x + i)] = 
            vga_entry(game_over[i], vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
    }
    
    // Score text
    char score_text[20] = "Your score: ";
    char score_str[10];
    snake_int_to_str(score, score_str);
    
    int i = 12; // Start after "Your score: "
    int j = 0;
    while (score_str[j] != '\0') {
        score_text[i++] = score_str[j++];
    }
    score_text[i] = '\0';
    
    start_x = (GAME_WIDTH - i) / 2;
    
    for (i = 0; score_text[i] != '\0'; i++) {
        terminal_buffer[9 * VGA_WIDTH + (start_x + i)] = 
            vga_entry(score_text[i], vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
    }
    
    // Instructions
    const char* restart_text = "Press ENTER to restart or ESC to exit";
    start_x = (GAME_WIDTH - 36) / 2;
    
    for (i = 0; restart_text[i] != '\0'; i++) {
        terminal_buffer[12 * VGA_WIDTH + (start_x + i)] = 
            vga_entry(restart_text[i], vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
    }
}

// Helper function to convert int to string (local version to avoid conflicts)
static void snake_int_to_str(int num, char* str) {
    int i = 0;
    bool is_negative = false;
    
    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    
    // Handle negative numbers
    if (num < 0) {
        is_negative = true;
        num = -num;
    }
    
    // Process individual digits
    while (num != 0) {
        str[i++] = (num % 10) + '0';
        num = num / 10;
    }
    
    // Add negative sign if needed
    if (is_negative) {
        str[i++] = '-';
    }
    
    // Add null terminator
    str[i] = '\0';
    
    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}