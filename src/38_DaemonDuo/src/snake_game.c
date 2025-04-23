#include "snake_game.h"
#include "terminal.h"
#include "pit.h"
#include "song_player.h"
#include "frequencies.h"
#include "idt.h"
#include <libc/stdlib.h>

// Snake state
static SnakeSegment snake[MAX_SNAKE_LENGTH];
static int snake_length;
static int direction;
static bool game_over;

// Food position
static int food_x, food_y;

// Latest key pressed
static volatile int last_key = 0;

// Game area buffer
static char game_area[GAME_HEIGHT][GAME_WIDTH + 1];  // +1 for null terminator

// Score
static int score;

// Game speed (milliseconds per frame)
static int game_speed = 200;

// Game state
static bool snake_game_active = false;
static uint32_t last_update_time = 0;

// Function to handle keyboard input in snake game
void snake_handle_input(char key) {
    last_key = key;
}

// Set up a new game
static void init_game() {
    // Initialize snake in the middle of the screen
    snake_length = 3;
    snake[0].x = GAME_WIDTH / 2;
    snake[0].y = GAME_HEIGHT / 2;
    snake[1].x = snake[0].x - 1;
    snake[1].y = snake[0].y;
    snake[2].x = snake[1].x - 1;
    snake[2].y = snake[1].y;
    
    // Initialize direction to right
    direction = DIR_RIGHT;
    
    // Place initial food
    food_x = 5;
    food_y = 5;
    
    // Reset game state
    game_over = false;
    score = 0;
    
    // Clear last key
    last_key = 0;
    
    // Initialize game area buffer with null terminators
    for (int y = 0; y < GAME_HEIGHT; y++) {
        game_area[y][GAME_WIDTH] = '\0';
    }
}

// Generate new food at a random position
static void spawn_food() {
    // Simple "random" placement - in a real game this would use a proper RNG
    food_x = (food_x * 7 + 13) % (GAME_WIDTH - 2) + 1;
    food_y = (food_y * 5 + 7) % (GAME_HEIGHT - 2) + 1;
    
    // Make sure food doesn't spawn on snake
    for (int i = 0; i < snake_length; i++) {
        if (snake[i].x == food_x && snake[i].y == food_y) {
            // Try again if it lands on the snake
            spawn_food();
            return;
        }
    }
}

// Update game state
static void update_game() {
    // Process input
    switch (last_key) {
        case 'w':
            if (direction != DIR_DOWN) direction = DIR_UP;
            break;
        case 'd':
            if (direction != DIR_LEFT) direction = DIR_RIGHT;
            break;
        case 's':
            if (direction != DIR_UP) direction = DIR_DOWN;
            break;
        case 'a':
            if (direction != DIR_RIGHT) direction = DIR_LEFT;
            break;
        case 'q':
            game_over = true;
            return;
    }
    
    // Clear last key
    last_key = 0;
    
    // Calculate new head position
    int new_x = snake[0].x;
    int new_y = snake[0].y;
    
    switch (direction) {
        case DIR_UP:
            new_y--;
            break;
        case DIR_RIGHT:
            new_x++;
            break;
        case DIR_DOWN:
            new_y++;
            break;
        case DIR_LEFT:
            new_x--;
            break;
    }
    
    // Check for collisions with walls
    if (new_x < 0 || new_x >= GAME_WIDTH || new_y < 0 || new_y >= GAME_HEIGHT) {
        game_over = true;
        return;
    }
    
    // Check for collisions with self
    for (int i = 0; i < snake_length; i++) {
        if (snake[i].x == new_x && snake[i].y == new_y) {
            game_over = true;
            return;
        }
    }
    
    // Move the snake (shift all segments down)
    for (int i = snake_length - 1; i > 0; i--) {
        snake[i].x = snake[i-1].x;
        snake[i].y = snake[i-1].y;
    }
    
    // Update head position
    snake[0].x = new_x;
    snake[0].y = new_y;
    
    // Check for food
    if (new_x == food_x && new_y == food_y) {
        // Increment score
        score++;
        
        // Play eating sound
        play_eating_sound();
        
        // Grow snake
        if (snake_length < MAX_SNAKE_LENGTH) {
            // Add a new segment at the end (initially at the same position as the last segment)
            snake[snake_length].x = snake[snake_length-1].x;
            snake[snake_length].y = snake[snake_length-1].y;
            snake_length++;
            
            // Speed up the game slightly
            if (game_speed > 50) {
                game_speed -= 5;
            }
        }
        
        // Spawn new food
        spawn_food();
    }
}

// Draw the game state to the game buffer
static void draw_game() {
    // Clear game area
    for (int y = 0; y < GAME_HEIGHT; y++) {
        for (int x = 0; x < GAME_WIDTH; x++) {
            if (x == 0 || x == GAME_WIDTH - 1 || y == 0 || y == GAME_HEIGHT - 1) {
                game_area[y][x] = '#'; // Draw border
            } else {
                game_area[y][x] = ' '; // Clear inner area
            }
        }
    }
    
    // Draw snake
    for (int i = 0; i < snake_length; i++) {
        if (snake[i].x >= 0 && snake[i].x < GAME_WIDTH && 
            snake[i].y >= 0 && snake[i].y < GAME_HEIGHT) {
            if (i == 0) {
                game_area[snake[i].y][snake[i].x] = 'O'; // Head
            } else {
                game_area[snake[i].y][snake[i].x] = 'o'; // Body
            }
        }
    }
    
    // Draw food
    if (food_x >= 0 && food_x < GAME_WIDTH && 
        food_y >= 0 && food_y < GAME_HEIGHT) {
        game_area[food_y][food_x] = '*';
    }
}

// Render the game to the terminal
static void render_game() {
    terminal_clear();
    
    // Print game title
    writeline("Snake Game - Use WASD to move, Q to quit\n\n");
    
    // Print score
    printf("Score: %d   Length: %d\n\n", score, snake_length);
    
    // Print game area
    for (int y = 0; y < GAME_HEIGHT; y++) {
        writeline(game_area[y]);
        terminal_putchar('\n');
    }
    
    // Print controls reminder
    writeline("\nControls: W=up, A=left, S=down, D=right, Q=quit\n");
}

// Play a sound when snake eats food
void play_eating_sound() {
    // Play a quick ascending sound
    play_sound(C5);
    sleep_interrupt(50);
    play_sound(E5);
    sleep_interrupt(50);
    play_sound(G5);
    sleep_interrupt(50);
    play_sound(C6);
    sleep_interrupt(100);
    stop_sound();
}

// Check if the snake game is active
bool is_snake_game_active() {
    return snake_game_active;
}

// Process any pending game updates
void process_pending_tasks() {
    if (snake_game_active) {
        uint32_t current_time = tick_count;
        
        // Check if it's time to update the game
        if (current_time - last_update_time >= game_speed) {
            // Update the game state
            update_game();
            
            // Draw the game to the buffer
            draw_game();
            
            // Render to the screen
            render_game();
            
            // Update the last update time
            last_update_time = current_time;
            
            // Check if the game is over
            if (game_over) {
                // Disable speaker
                disable_speaker();
                
                // Show game over message
                terminal_clear();
                writeline("Game Over!\n\n");
                printf("Final Score: %d\n", score);
                printf("Snake Length: %d\n\n", snake_length);
                
                writeline("Press any key to return to terminal...\n");
                
                // Reset the game state
                snake_game_active = false;
                
                // Exit snake game mode
                set_snake_game_mode(false);
                
                // Reset the PIT to make sure timer interrupts work again
                reset_pit_timer();
                
                // Make sure keyboard IRQ is enabled
                enable_irq(1);
                
                // Give it a moment to finish
                sleep_interrupt(500);
                
                // Clear screen and return to terminal
                terminal_clear();
                writeline("daemon-duo> ");
            }
        }
    }
}

// Main snake game loop - now non-blocking
void start_snake_game() {
    // Reset any variables that might persist between game sessions
    snake_game_active = false;
    game_over = false;
    
    // Tell the IDT we're in snake game mode
    set_snake_game_mode(true);
    
    // Clear screen and show starting message
    terminal_clear();
    writeline("Starting Snake Game...\n");

    // Make sure the keyboard IRQ is enabled
    enable_irq(1);
    
    // A longer sleep to ensure the system stabilizes
    sleep_interrupt(1000);
    
    // Reset tick counter to avoid timing issues
    last_update_time = tick_count;
    
    // Set up the game
    init_game();
    
    // Draw the initial game state
    draw_game();
    
    // Render the initial screen
    render_game();
    
    // Make sure interrupts are still enabled after rendering
    asm volatile("sti");
    
    // Enable speaker
    enable_speaker();
    
    // Set the game as active - do this AFTER all setup is complete
    snake_game_active = true;
    
    // Play a startup sound
    play_eating_sound();
}
