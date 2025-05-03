#include "snake.h"
#include "terminal.h"
#include "memory.h"
#include "pit.h"
#include "io.h"
#include "libc/stdio.h"

// Forward declarations of static functions
static void init_impl(void);
static void update_impl(void);
static void handle_input_impl(uint8_t key);
static void draw_char(uint8_t x, uint8_t y, char c);
static void clear_board(void);
static void draw_border(void);
static bool check_collision(uint8_t x, uint8_t y);
static void place_food(void);
static void render_score(void);
static void update_snake(void);
static void cleanup_snake(void);
static void game_over(void);
static void draw_title(void);

// Display constants
#define BOARD_OFFSET_X    2
#define BOARD_OFFSET_Y    5
#define BORDER_CHAR       '#'
#define SNAKE_HEAD_CHAR   'O'
#define SNAKE_BODY_CHAR   'o'
#define FOOD_CHAR         '+'
#define EMPTY_CHAR        ' '
#define TICK_INCREMENT    10   // ms per timer tick

// Snake node structure
typedef struct SnakeNode {
    uint8_t x;
    uint8_t y;
    struct SnakeNode* next;
} SnakeNode;

// Game state
static struct {
    SnakeNode* head;
    SnakeNode* tail;
    uint8_t food_x;
    uint8_t food_y;
    Direction direction;
    Direction next_direction;  // Buffered direction change
    GameState state;
    uint32_t score;
    uint32_t tick_accumulator;
    uint32_t speed;
    bool growing;
    uint8_t color_index;       // For color cycling
} game_state;

// Color definitions
#define COLOR_BACKGROUND  0x00  // Black
#define COLOR_BORDER      0x09  // Bright Blue
#define COLOR_SNAKE_HEAD  0x0A  // Bright Green
#define COLOR_SNAKE_BODY  0x02  // Green
#define COLOR_FOOD        0x0C  // Bright Red
#define COLOR_TITLE       0x0E  // Yellow
#define COLOR_SCORE       0x0B  // Bright Cyan
#define COLOR_GAME_OVER   0x0C  // Bright Red

// Implementation functions
static void init_impl(void) {
    // Enable game mode
    set_game_mode(true);
    
    // Clean up any existing snake
    cleanup_snake();
    
    // Reset game state
    game_state.score = 0;
    game_state.direction = DIRECTION_RIGHT;
    game_state.next_direction = DIRECTION_RIGHT;
    game_state.state = GAME_STATE_RUNNING;
    game_state.tick_accumulator = 0;
    game_state.speed = SNAKE_TICK_MS;
    game_state.growing = false;
    game_state.color_index = 0;

    // Clear and set background color
    terminal_clear();
    
    // Draw game elements
    draw_title();
    draw_border();
    clear_board();  // Clear the play area with background color

    // Create initial snake
    SnakeNode* node = malloc(sizeof(SnakeNode));
    if (!node) return;
    
    node->x = SNAKE_START_X;
    node->y = SNAKE_START_Y;
    node->next = NULL;
    game_state.head = game_state.tail = node;
    
    // Add two more initial segments to make the snake visible
    for (int i = 0; i < 2; i++) {
        SnakeNode* new_tail = malloc(sizeof(SnakeNode));
        if (!new_tail) break;
        
        new_tail->x = SNAKE_START_X - (i + 1);
        new_tail->y = SNAKE_START_Y;
        new_tail->next = NULL;
        
        game_state.tail->next = new_tail;
        game_state.tail = new_tail;
    }
    
    // Draw initial snake
    SnakeNode* current = game_state.head;
    while (current != NULL) {
        if (current == game_state.head) {
            terminal_set_color(COLOR_SNAKE_HEAD);
            draw_char(current->x, current->y, SNAKE_HEAD_CHAR);
        } else {
            terminal_set_color(COLOR_SNAKE_BODY);
            draw_char(current->x, current->y, SNAKE_BODY_CHAR);
        }
        current = current->next;
    }
    
    // Reset color
    terminal_set_color(0x07); // Default color
    
    // Place initial food and show score
    place_food();
    render_score();
}

static void update_impl(void) {
    // This function will be called every frame
    if (game_state.state == GAME_STATE_RUNNING) {
        render_score(); // Update score display every frame
    }
}

static void handle_input_impl(uint8_t key) {
    // Ignore input if game is over (except restart/exit)
    if (game_state.state == GAME_STATE_OVER && 
        key != SCANCODE_R && key != SCANCODE_ESC) {
        return;
    }

    switch (key) {
        case SCANCODE_UP:
            if (game_state.direction != DIRECTION_DOWN)
                game_state.next_direction = DIRECTION_UP;
            break;
        case SCANCODE_DOWN:
            if (game_state.direction != DIRECTION_UP)
                game_state.next_direction = DIRECTION_DOWN;
            break;
        case SCANCODE_LEFT:
            if (game_state.direction != DIRECTION_RIGHT)
                game_state.next_direction = DIRECTION_LEFT;
            break;
        case SCANCODE_RIGHT:
            if (game_state.direction != DIRECTION_LEFT)
                game_state.next_direction = DIRECTION_RIGHT;
            break;
        case SCANCODE_P:
            if (game_state.state == GAME_STATE_RUNNING)
                game_state.state = GAME_STATE_PAUSED;
            else if (game_state.state == GAME_STATE_PAUSED)
                game_state.state = GAME_STATE_RUNNING;
            break;
        case SCANCODE_R:
            if (game_state.state == GAME_STATE_OVER)
                init_impl();
            break;
    }
}

// Helper functions
static void draw_char(uint8_t x, uint8_t y, char c) {
    int screen_y = BOARD_OFFSET_Y + y;
    int screen_x = BOARD_OFFSET_X + x;
    
    // Boundary check to prevent drawing outside the playable area
    if (screen_y >= 0 && screen_y < VGA_HEIGHT && 
        screen_x >= 0 && screen_x < VGA_WIDTH) {
        terminal_set_cursor(screen_y, screen_x);
        terminal_put_char(c);
    }
}

static void clear_board(void) {
    terminal_set_color(COLOR_BACKGROUND);
    for (uint8_t y = 0; y < SNAKE_BOARD_HEIGHT; y++) {
        for (uint8_t x = 0; x < SNAKE_BOARD_WIDTH; x++) {
            draw_char(x, y, EMPTY_CHAR);
        }
    }
    terminal_set_color(0x07); // Reset color
}

static void draw_title(void) {
    const int title_row = 1;
    terminal_set_color(COLOR_TITLE);
    
    // Draw centered title box
    terminal_write_centered(title_row, "S N A K E");
    
    terminal_set_color(0x07); // Reset color
}

static void draw_border(void) {
    terminal_set_color(COLOR_BORDER);

    // Calculate proper dimensions for the play area
    int top_row = BOARD_OFFSET_Y - 1;
    int bottom_row = BOARD_OFFSET_Y + SNAKE_BOARD_HEIGHT -1 ; // Fixed bottom border position
    int left_col = BOARD_OFFSET_X - 1;
    int right_col = BOARD_OFFSET_X + SNAKE_BOARD_WIDTH - 1;
    int width = right_col - left_col + 1;

    // Draw the top border (horizontal line)
    for (int x = 0; x < width; x++) {
        terminal_set_cursor(top_row, left_col + x);
        terminal_put_char('═');
    }

    // Draw the bottom border (horizontal line)
    for (int x = 0; x < width; x++) {
        terminal_set_cursor(bottom_row, left_col + x );
        terminal_put_char('i');
    }

    // Draw the left border (vertical line)
    for (int y = 0; y < SNAKE_BOARD_HEIGHT; y++) {
        terminal_set_cursor(top_row + 1 + y, left_col);
        terminal_put_char('║');
    }

    // Draw the right border (vertical line)
    for (int y = 0; y < SNAKE_BOARD_HEIGHT; y++) {
        terminal_set_cursor(top_row + 1 + y, right_col + 1);
        terminal_put_char('║');
    }

    // Draw the corners
    terminal_set_cursor(top_row, left_col);
    terminal_put_char('╔');
    terminal_set_cursor(top_row, right_col + 1);
    terminal_put_char('╗');
    terminal_set_cursor(bottom_row , left_col);
    terminal_put_char('╚');
    terminal_set_cursor(bottom_row, right_col + 1);
    terminal_put_char('╝');

    terminal_set_color(0x07); // Reset color
}

static void place_food(void) {
    static uint32_t seed = 12345;
    uint8_t x, y;
    bool valid;
    
    do {
        seed = (seed * 1103515245 + 12345) & 0x7fffffff;
        x = (seed % (SNAKE_BOARD_WIDTH - 2)) + 1;
        // Adjust for the corrected board height
        y = ((seed >> 16) % (SNAKE_BOARD_HEIGHT - 2)) + 1;
        
        // Make sure food is within visible play area
        if (y >= SNAKE_BOARD_HEIGHT - 1) {
            y = SNAKE_BOARD_HEIGHT - 2;
        }
        
        // Check if position is free
        valid = true;
        SnakeNode* current = game_state.head;
        while (current != NULL) {
            if (current->x == x && current->y == y) {
                valid = false;
                break;
            }
            current = current->next;
        }
    } while (!valid);
    
    game_state.food_x = x;
    game_state.food_y = y;
    
    terminal_set_color(COLOR_FOOD);
    draw_char(x, y, FOOD_CHAR);
    terminal_set_color(0x07); // Reset color
}

static void render_score(void) {
    // Clear the score area first with background color
    terminal_set_color(COLOR_BACKGROUND);
    for (int x = 0; x < SNAKE_BOARD_WIDTH; x++) {
        terminal_set_cursor(BOARD_OFFSET_Y - 2, BOARD_OFFSET_X + x);
        terminal_put_char(' ');
    }
    
    // Display score on the left
    terminal_set_cursor(BOARD_OFFSET_Y - 2, BOARD_OFFSET_X);
    terminal_set_color(COLOR_SCORE);
    printf("Score: %u", game_state.score);
    
    // Show controls on the right side
    terminal_set_cursor(BOARD_OFFSET_Y - 2, BOARD_OFFSET_X + SNAKE_BOARD_WIDTH - 18);
    printf("P:Pause R:Restart");
    
    terminal_set_color(0x07); // Reset color
}

static bool check_collision(uint8_t x, uint8_t y) {
    // Border collision - adjust collision detection to match the new border
    if (x == 0 || x >= SNAKE_BOARD_WIDTH - 1 ||
        y == 0 || y >= SNAKE_BOARD_HEIGHT) {
        return true;
    }
    
    // Self collision (skip head)
    SnakeNode* current = game_state.head->next;
    while (current != NULL) {
        if (current->x == x && current->y == y) {
            return true;
        }
        current = current->next;
    }
    
    return false;
}

static void cleanup_snake(void) {
    SnakeNode* current = game_state.head;
    while (current != NULL) {
        SnakeNode* next = current->next;
        free(current);
        current = next;
    }
    game_state.head = game_state.tail = NULL;
}

static void game_over(void) {
    game_state.state = GAME_STATE_OVER;
    
    // Draw game over message
    terminal_set_color(COLOR_GAME_OVER);
    terminal_set_cursor(BOARD_OFFSET_Y + SNAKE_BOARD_HEIGHT/2 - 1, BOARD_OFFSET_X + SNAKE_BOARD_WIDTH/2 - 5);
    printf("GAME OVER!");
    terminal_set_cursor(BOARD_OFFSET_Y + SNAKE_BOARD_HEIGHT/2, BOARD_OFFSET_X + SNAKE_BOARD_WIDTH/2 - 6);
    printf("Score: %u", game_state.score);
    terminal_set_cursor(BOARD_OFFSET_Y + SNAKE_BOARD_HEIGHT/2 + 1, BOARD_OFFSET_X + SNAKE_BOARD_WIDTH/2 - 10);
    printf("Press R to restart");
    
    terminal_set_color(0x07); // Reset color
}

static void update_snake(void) {
    if (game_state.state != GAME_STATE_RUNNING) {
        return;
    }

    // Apply buffered direction change
    game_state.direction = game_state.next_direction;
    
    uint8_t new_x = game_state.head->x;
    uint8_t new_y = game_state.head->y;
    
    switch (game_state.direction) {
        case DIRECTION_UP:    new_y--; break;
        case DIRECTION_DOWN:  new_y++; break;
        case DIRECTION_LEFT:  new_x--; break;
        case DIRECTION_RIGHT: new_x++; break;
    }
    
    if (check_collision(new_x, new_y)) {
        game_over();
        return;
    }
    
    // Create new head
    SnakeNode* new_head = malloc(sizeof(SnakeNode));
    if (!new_head) {
        game_over();
        return;
    }
    
    new_head->x = new_x;
    new_head->y = new_y;
    new_head->next = game_state.head;
    game_state.head = new_head;
    
    // Change old head to body
    terminal_set_color(COLOR_SNAKE_BODY);
    draw_char(game_state.head->next->x, game_state.head->next->y, SNAKE_BODY_CHAR);
    
    // Check if food was eaten
    if (new_x == game_state.food_x && new_y == game_state.food_y) {
        game_state.score++;
        game_state.growing = true;
        render_score();
        
        // Increase speed every 5 points
        if (game_state.score % 5 == 0 && game_state.speed > 20) {
            game_state.speed -= 5;
        }
        
        place_food();
    } else {
        game_state.growing = false;
    }
    
    // If not growing, remove tail
    if (!game_state.growing) {
        SnakeNode* current = game_state.head;
        while (current->next != game_state.tail) {
            current = current->next;
        }
        
        draw_char(game_state.tail->x, game_state.tail->y, EMPTY_CHAR);
        free(game_state.tail);
        current->next = NULL;
        game_state.tail = current;
    }
    
    // Draw new head
    terminal_set_color(COLOR_SNAKE_HEAD);
    draw_char(new_x, new_y, SNAKE_HEAD_CHAR);
    terminal_set_color(0x07); // Reset color
}

// Required callback functions
void snake_tick(void) {
    if (game_state.state == GAME_STATE_RUNNING) {
        terminal_set_cursor(0, 0);
        terminal_write("Ticking...");
        game_state.tick_accumulator += TICK_INCREMENT;
        if (game_state.tick_accumulator >= game_state.speed) {
            terminal_set_cursor(1, 0);
            terminal_write("Updating snake...");
            game_state.tick_accumulator = 0;
            update_snake();
        }
    }
}


void snake_on_key(uint8_t scancode) {
    handle_input_impl(scancode);
    
    // Make the snake move on key press if it's currently paused
    // This is for better responsiveness
    if (game_state.state == GAME_STATE_RUNNING) {
        update_snake();
    }
}

// Game instance
static SnakeGame snake_game = {
    .init = init_impl,
    .update = update_impl,
    .handle_input = handle_input_impl,
    .get_score = NULL,
    .get_state = NULL
};

SnakeGame* create_snake_game(void) {
    return &snake_game;
}