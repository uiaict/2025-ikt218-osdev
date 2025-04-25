#include "snake.h"
#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "display.h"
#include "interruptHandler.h"
#include "programmableIntervalTimer.h"
#include "pcSpeaker.h"
#include "memory_manager.h"
#include "miscFuncs.h"

// Direct access to VGA memory
static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

// Screen dimensions
static const int SCREEN_WIDTH = 80;  // Matches VGA_WIDTH in display.c
static const int SCREEN_HEIGHT = 25;
static const int VGA_WIDTH = 80;     // Used with VGA_MEMORY indexing

// Game area dimensions (excluding borders)
static const int GAME_WIDTH = 78;
static const int GAME_HEIGHT = 23;

// Initial snake position
static const int INITIAL_X = 40;
static const int INITIAL_Y = 12;

// Random number seed
static uint32_t random_seed = 12345;

// Simple string functions implementation
static size_t my_strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

static char* my_strcpy(char* dest, const char* src) {
    char* d = dest;
    while ((*dest++ = *src++));
    return d;
}

// Create a VGA entry with the given character and color (copied from display.c)
static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

// Create a VGA color byte from foreground and background colors (copied from display.c)
static inline uint8_t vga_color(vga_color_t fg, vga_color_t bg) {
    return fg | (bg << 4);
}

// Generate a simple random number
static uint32_t rand(void) {
    random_seed = random_seed * 1103515245 + 12345;
    return (random_seed / 65536) % 32768;
}

// Initialize the snake game
void snake_init(void) {
    // Reset the random seed using the timer tick
    random_seed = get_current_tick();
}

// Show the snake game menu/instructions
void show_snake_menu(void) {
    display_clear();
    
    display_write_color("\n\n", COLOR_WHITE);
    display_write_color("                           SNAKE GAME\n\n", COLOR_LIGHT_GREEN);
    display_write_color("                     Control using WASD keys:\n\n", COLOR_YELLOW);
    display_write_color("                           W = Up\n", COLOR_WHITE);
    display_write_color("                     A = Left    D = Right\n", COLOR_WHITE);
    display_write_color("                           S = Down\n\n", COLOR_WHITE);
    display_write_color("                    Press ESC to quit game\n\n", COLOR_WHITE);
    display_write_color("                Press any key to start playing...\n", COLOR_LIGHT_CYAN);
    
    // Wait for keypress
    while (!keyboard_data_available()) {
        __asm__ volatile("hlt");
    }
    keyboard_getchar(); // Clear the keypress
    
    display_clear();
}

// Initialize game state
static void init_game_state(GameState* state) {
    // Set initial snake position
    state->snake.length = 3;
    state->snake.is_alive = true;
    state->snake.direction = RIGHT;
    
    // Starting in the middle of the screen
    int start_x = SCREEN_WIDTH / 2;
    int start_y = SCREEN_HEIGHT / 2;
    
    // Create initial snake segments (head at 0)
    for (int i = 0; i < state->snake.length; i++) {
        state->snake.segments[i].x = start_x - i;
        state->snake.segments[i].y = start_y;
    }
    
    // Generate initial apple
    generate_apple(state);
    
    // Set game speed (higher = slower)
    state->game_speed = 200;  // Slowed down from 100 to 200 for better playability
    
    // Initialize score and game state
    state->score = 0;
    state->game_over = false;
}

// Generate a new apple position
void generate_apple(GameState* state) {
    bool valid_position = false;
    
    while (!valid_position) {
        // Generate a random position within the game area
        state->apple.x = 1 + (rand() % (GAME_WIDTH - 2));
        state->apple.y = 1 + (rand() % (GAME_HEIGHT - 2));
        
        // Check if the position overlaps with the snake
        valid_position = true;
        for (int i = 0; i < state->snake.length; i++) {
            if (state->apple.x == state->snake.segments[i].x && 
                state->apple.y == state->snake.segments[i].y) {
                valid_position = false;
                break;
            }
        }
    }
}

// Draw the game screen
void draw_game_screen(const GameState* state) {
    display_clear();
    
    // Draw the border
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        display_set_cursor(x, 0);
        display_write_char_color('-', COLOR_LIGHT_CYAN);  // Toppramme
        
        display_set_cursor(x, SCREEN_HEIGHT - 1);
        display_write_char_color('-', COLOR_LIGHT_CYAN);  // Bunnramme
    }
    
    for (int y = 1; y < SCREEN_HEIGHT - 1; y++) {
        display_set_cursor(0, y);
        display_write_char_color('|', COLOR_LIGHT_CYAN);  // Venstreramme
        
        display_set_cursor(SCREEN_WIDTH - 1, y);
        display_write_char_color('|', COLOR_LIGHT_CYAN);  // Høyreramme
    }
    
    // Draw corners with plus signs
    display_set_cursor(0, 0);
    display_write_char_color('+', COLOR_LIGHT_CYAN);  // Øvre venstre hjørne
    
    display_set_cursor(SCREEN_WIDTH - 1, 0);
    display_write_char_color('+', COLOR_LIGHT_CYAN);  // Øvre høyre hjørne
    
    display_set_cursor(0, SCREEN_HEIGHT - 1);
    display_write_char_color('+', COLOR_LIGHT_CYAN);  // Nedre venstre hjørne
    
    display_set_cursor(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    display_write_char_color('+', COLOR_LIGHT_CYAN);  // Nedre høyre hjørne
    
    // Draw the snake
    for (int i = 0; i < state->snake.length; i++) {
        int x = state->snake.segments[i].x;
        int y = state->snake.segments[i].y;
        
        // Only draw if within bounds
        if (x > 0 && x < SCREEN_WIDTH - 1 && y > 0 && y < SCREEN_HEIGHT - 1) {
            display_set_cursor(x, y);
            if (i == 0) {
                display_write_char_color(SNAKE_HEAD_CHAR, COLOR_LIGHT_GREEN);  // Head
            } else {
                display_write_char_color(SNAKE_CHAR, COLOR_GREEN);  // Body
            }
        }
    }
    
    // Draw the apple
    if (state->apple.x > 0 && state->apple.x < SCREEN_WIDTH - 1 && 
        state->apple.y > 0 && state->apple.y < SCREEN_HEIGHT - 1) {
        display_set_cursor(state->apple.x, state->apple.y);
        display_write_char_color(APPLE_CHAR, COLOR_LIGHT_RED);
    }
    
    // Draw the score - på en fast plass utenfor spillområdet
    display_set_cursor(2, SCREEN_HEIGHT);
    display_write_color("Score: ", COLOR_YELLOW);
    
    // Convert score to string
    char score_text[16];
    int_to_string(state->score, score_text);
    display_write_color(score_text, COLOR_YELLOW);
    
    // If game over, display message
    if (state->game_over) {
        const char* game_over_text = "GAME OVER!";
        int text_len = my_strlen(game_over_text);
        int center_x = (SCREEN_WIDTH - text_len) / 2;
        int center_y = SCREEN_HEIGHT / 2;
        
        display_set_cursor(center_x, center_y);
        display_write_color(game_over_text, COLOR_LIGHT_RED);
        
        const char* restart_text = "Press 'R' to restart or ESC to exit";
        int restart_len = my_strlen(restart_text);
        center_x = (SCREEN_WIDTH - restart_len) / 2;
        
        display_set_cursor(center_x, center_y + 2);
        display_write_color(restart_text, COLOR_LIGHT_CYAN);
    }
    
    // Hide the cursor at the end
    display_hide_cursor();
}

// Process keyboard input - simplified and optimized
Direction process_input(Direction current_direction, bool* quit_game) {
    Direction new_direction = current_direction;
    *quit_game = false;
    
    // Only check the keyboard buffer - more efficient
    while (keyboard_data_available()) {
        char key = keyboard_getchar();
        
        // Handle escape key
        if (key == 27) {
            *quit_game = true;
            return current_direction;
        }
        
        // Handle directional movement - simplified logic
        switch (key) {
            case 'w': case 'W': case 'i': case 'I':
                if (current_direction != DOWN) new_direction = UP;
                break;
                
            case 's': case 'S': case 'k': case 'K':
                if (current_direction != UP) new_direction = DOWN;
                break;
                
            case 'a': case 'A': case 'j': case 'J':
                if (current_direction != RIGHT) new_direction = LEFT;
                break;
                
            case 'd': case 'D': case 'l': case 'L':
                if (current_direction != LEFT) new_direction = RIGHT;
                break;
        }
    }
    
    return new_direction;
}

// Check if there's a collision
bool check_collision(const GameState* state) {
    const Position* head = &state->snake.segments[0];
    
    // Check collision with walls
    if (head->x <= 0 || head->x >= SCREEN_WIDTH - 1 || 
        head->y <= 0 || head->y >= SCREEN_HEIGHT - 1) {
        return true;
    }
    
    // Check collision with self
    for (int i = 1; i < state->snake.length; i++) {
        if (head->x == state->snake.segments[i].x && 
            head->y == state->snake.segments[i].y) {
            return true;
        }
    }
    
    return false;
}

// Move the snake in the current direction
void move_snake(GameState* state) {
    // Flagg for å spore status
    static bool already_played_game_over_sound = false;
    
    // Remember the old position of each segment
    Position old_segments[SNAKE_MAX_LENGTH];
    for (int i = 0; i < state->snake.length; i++) {
        old_segments[i] = state->snake.segments[i];
    }
    
    // Move the head based on current direction
    switch (state->snake.direction) {
        case UP:
            state->snake.segments[0].y--;
            break;
        case DOWN:
            state->snake.segments[0].y++;
            break;
        case LEFT:
            state->snake.segments[0].x--;
            break;
        case RIGHT:
            state->snake.segments[0].x++;
            break;
    }
    
    // Move the rest of the body
    for (int i = 1; i < state->snake.length; i++) {
        state->snake.segments[i] = old_segments[i - 1];
    }
    
    // Check if snake eats the apple - play sound IMMEDIATELY when apple is reached
    if (state->snake.segments[0].x == state->apple.x && 
        state->snake.segments[0].y == state->apple.y) {
        
        // Direkte programmering av PIT og høyttaler for eple-lyd
        uint16_t divisor = 1193180 / 1000;  // 1000Hz tone
        
        // Disable interrupts while manipulating speaker
        __asm__ volatile("cli");
        
        // Configure PIT channel 2 for square wave
        outb(0x43, 0xB6);
        outb(0x42, divisor & 0xFF);
        outb(0x42, (divisor >> 8) & 0xFF);
        
        // Turn speaker on
        uint8_t tmp = inb(0x61);
        outb(0x61, tmp | 3);
        
        // Re-enable interrupts
        __asm__ volatile("sti");
        
        // Play sound for very short duration with minimal CPU usage
        // This prevents the sound from causing game lag
        uint32_t start_time = get_current_tick();
        while (get_current_tick() - start_time < 1) {
            __asm__ volatile("pause");
        }
        
        // Turn speaker off directly
        tmp = inb(0x61);
        outb(0x61, tmp & ~3);
        
        // Increase score
        state->score++;
        
        // Increase length if there's room
        if (state->snake.length < SNAKE_MAX_LENGTH) {
            state->snake.length++;
            
            // Add a new segment at the end (same position as previous end, will move on next update)
            state->snake.segments[state->snake.length - 1] = 
                old_segments[state->snake.length - 2];
        }
        
        // Generate a new apple
        generate_apple(state);
    
    }
    
    // Check for collisions
    if (check_collision(state)) {
        state->snake.is_alive = false;
        state->game_over = true;
        
        // Spill game over-lyden KUN én gang og med definert varighet
        if (!already_played_game_over_sound) {
            // Set flag first to prevent duplicates
            already_played_game_over_sound = true;
            
            // Disable interrupts while setting up sound
            __asm__ volatile("cli");
            
            // Game over lyd - 200Hz 
            uint16_t divisor = 1193180 / 200;
            outb(0x43, 0xB6);
            outb(0x42, divisor & 0xFF);
            outb(0x42, (divisor >> 8) & 0xFF);
            
            // Turn speaker on
            uint8_t tmp = inb(0x61);
            outb(0x61, tmp | 3);
            
            // Re-enable interrupts
            __asm__ volatile("sti");
            
            // Spill lyden i 200ms med effektiv venting
            uint32_t start_time = get_current_tick();
            // Med PIT frekvens på 1000Hz, 200 ticks = 200ms
            while (get_current_tick() - start_time < 200) {
                __asm__ volatile("pause");
            }
            
            // Stopp lyden eksplisitt
            tmp = inb(0x61);
            outb(0x61, tmp & ~3);  // Turn speaker off
        }
    } else {
        // Reset flag when not in game over state
        already_played_game_over_sound = false;
    }
}

// Update the game state
void update_game(GameState* state, Direction input_direction) {
    // Update snake direction
    state->snake.direction = input_direction;
    
    // Move the snake
    move_snake(state);
}

// Handle the main snake game loop
void handle_snake_game(void) {
    show_snake_menu();
    
    // Initialize the game state
    GameState game_state;
    init_game_state(&game_state);
    
    // Clear keyboard buffer
    while (keyboard_data_available()) {
        keyboard_getchar();
    }
    
    // Make sure interrupts are enabled
    __asm__ volatile("sti");
    
    bool running = true;
    uint32_t last_update_time = get_current_tick();
    uint32_t last_input_check = get_current_tick(); // Separate counter for input checking
    Direction new_direction = game_state.snake.direction;
    
    // Game timing:
    // - game_state.game_speed is set to 200 ms in init_game_state()
    // - This means the snake moves every 200 ms (5 frames per second)
    // - Input is checked every 1 ms for responsiveness
    
    while (running) {
        // Get current time
        uint32_t current_time = get_current_tick();
        
        // Process input frequently for good responsiveness
        if (current_time - last_input_check >= 1) {
            bool quit_requested = false;
            Direction input_direction = process_input(game_state.snake.direction, &quit_requested);
            
            // Save direction for next update
            if (input_direction != game_state.snake.direction) {
                new_direction = input_direction;
            }
            
            // Check if quit was requested
            if (quit_requested) {
                running = false;
                break;
            }
            
            last_input_check = current_time;
        }
        
        // Update game at specified intervals
        if (current_time - last_update_time >= game_state.game_speed) {
            if (!game_state.game_over) {
                update_game(&game_state, new_direction);
            }
            draw_game_screen(&game_state);
            last_update_time = current_time;
        }
        
        // If game over, wait for keypress to continue
        if (game_state.game_over) {
            sleep_interrupt(200);
            
            // Clear any buffered input
            while (keyboard_data_available()) {
                keyboard_getchar();
            }
            
            // Display "Press 'R' to restart or ESC to exit" message
            int text_len = 37; // Length of the message
            const char* restart_text = "Press 'R' to restart or ESC to exit";
            int start_x = (SCREEN_WIDTH - text_len) / 2;
            
            terminal_row = SCREEN_HEIGHT / 2 + 2;
            terminal_column = start_x;
            
            // Clear the line first
            for (int i = 0; i < text_len; i++) {
                display_write_char_color(' ', COLOR_BLACK);
            }
            
            // Display restart message
            terminal_column = start_x;
            for (int i = 0; i < text_len; i++) {
                display_write_char_color(restart_text[i], COLOR_YELLOW);
            }
            
            display_move_cursor();
            
            // Wait for 'R' key or ESC key
            bool restart = false;
            while (!restart && running) {
                // Check for keyboard input
                if (keyboard_data_available()) {
                    char key = keyboard_getchar();
                    if (key == 'r' || key == 'R') {
                        restart = true;
                    } else if (key == 27) { // ESC key
                        running = false;
                    }
                }
                
                // Small sleep to avoid excessive CPU usage
                sleep_interrupt(10);
            }
            
            // Clear any pending keystrokes
            while (keyboard_data_available()) {
                keyboard_getchar();
            }
            
            if (restart) {
                // Vis melding om at spillet starter på nytt
                display_clear();
                display_write_color("\n\n\n\n\n\n\n\n\n\n           Restarting game...", COLOR_LIGHT_GREEN);
                
                // Vent litt for å unngå umiddelbar kollisjon
                sleep_interrupt(500);
                
                // Lag en helt ny spilltilstand i stedet for å bruke memset
                game_state.snake.length = 0;
                game_state.snake.is_alive = false;
                game_state.game_over = false;
                game_state.score = 0;
                
                // Initialiser spillet på nytt med en frisk spilltilstand
                init_game_state(&game_state);
                
                // Tilbakestill retning og tidsverdier
                new_direction = game_state.snake.direction;
                last_update_time = get_current_tick();
                last_input_check = get_current_tick();
                
                // Clear the screen and draw the initial state
                display_clear();
                draw_game_screen(&game_state);
            }
        }
        
        // Small yield with timer-based pause rather than CPU busy waiting
        sleep_interrupt(1);
    }
    
    // Clear the screen before returning to menu
    display_clear();
} 