#include "apps/game/snake.h"
#include "kernel/pit.h"
#include "libc/string.h"
#include "libc/stdbool.h"
#include "kernel/memory.h"
#include "common/itoa.h" 

extern volatile int last_key;
// Sound flag
static bool sound_on = true; 
static bool game_start = true;   

// Holds the current state of the game
static SnakeGame game;

// Picks a random empty spot on the board that isn't occupied by the snake or food
static Point random_free_cell(void)
{
    // Build a list of "banned" tile indices (snake body + food)
    uint32_t banned[SNAKE_MAX_LENGTH + 1];
    size_t n = 0;

    for (int i = 0; i < game.snake.length; ++i)
        banned[n++] = TILE_IDX(game.snake.segments[i].x,
                               game.snake.segments[i].y);

    // Exclude food if it's already on the board
    if (game.food.x)
        banned[n++] = TILE_IDX(game.food.x, game.food.y);
    
    // Total number of usable tiles (excluding borders)
    uint32_t total = (SNAKE_BOARD_WIDTH  - 2) * (SNAKE_BOARD_HEIGHT - 2);
    uint32_t idx   = rand_range_skip(total, banned, n);
    
    // Convert 1D index back to 2D coordinates inside the play area
    Point p = {
        .x = 1 +  idx % (SNAKE_BOARD_WIDTH  - 2),
        .y = 1 +  idx / (SNAKE_BOARD_WIDTH  - 2)
    };
    return p;
}

// Places food on a random free cell and draws it on screen
static void draw_food(void)
{
    Point f = game.food;
    game_draw_char(OFFSET_X + f.x, OFFSET_Y + f.y, '*', CLR_YELLOW);
    game.tiles[f.y][f.x] = TILE_FOOD;
}

static void place_food(void)
{
    game.food = random_free_cell();
    draw_food();
}

// Draws the full game board, including borders and empty spaces
static void draw_board() {
    for (int y = 0; y < SNAKE_BOARD_HEIGHT; y++) {
        for (int x = 0; x < SNAKE_BOARD_WIDTH; x++) {
            if (y == 0 || y == SNAKE_BOARD_HEIGHT - 1 || x == 0 || x == SNAKE_BOARD_WIDTH - 1) {
                // Draw borders in green
                game_draw_char(OFFSET_X + x, OFFSET_Y + y, '#', CLR_GREEN); 
                game.tiles[y][x] = TILE_BORDER;
            } else {
                // Draw empty space
                game_draw_char(OFFSET_X + x, OFFSET_Y + y, ' ', 0x07); 
                game.tiles[y][x] = TILE_EMPTY;
            }
        }
    }
}

// Draws the score under the board
static void draw_score(void)
{
    // write the fixed label 
    game_draw_string(OFFSET_X, SCORE_ROW, "Score: ", CLR_NORMAL);

    // convert (tail-3) straight into a small buffer
    char num[12];
    itoa(game.snake.length - 3, num, 10);

    // print the digits right after the label
    game_draw_string(OFFSET_X + 7, SCORE_ROW, num, CLR_NORMAL);

    // pad with spaces to erase leftover digits from higher scores 
    game_draw_string(OFFSET_X + 7 + strlen(num), SCORE_ROW, "   ", CLR_NORMAL);
}

// Draws the snake on the board: head is capital 'O', body is smal 'o'
static void draw_snake() {
    for (int i = 0; i < game.snake.length; i++) {
        Point p = game.snake.segments[i];
        game_draw_char(OFFSET_X + p.x, OFFSET_Y + p.y, (i == 0) ? 'O' : 'o', CLR_NORMAL); // Head vs body
        game.tiles[p.y][p.x] = TILE_SNAKE;
    }
}

// Clears snake's previous position – now guards against accidentally clearing borders
static void clear_snake(void)
{
    for (int i = 0; i < game.snake.length; ++i) {
        Point p = game.snake.segments[i];
        // Only clear if inside the valid play area (skip borders)
        if (p.x > 0 && p.x < SNAKE_BOARD_WIDTH - 1 &&
            p.y > 0 && p.y < SNAKE_BOARD_HEIGHT - 1) {
            game_draw_char(OFFSET_X + p.x, OFFSET_Y + p.y, ' ', 0x07);
            game.tiles[p.y][p.x] = TILE_EMPTY;
        }
    }
}

// Initializes the game state with a new snake and a first piece of food
static void init_snake(void)
{
    memset(&game, 0, sizeof(SnakeGame)); // Reset everything

    // Place the snake's head at a free random location
    Point head = random_free_cell();

    // Pick a random starting direction
    int dir = rand_range(4);      /* 0-UP … 3-RIGHT */
    game.snake.direction = dir;
    game.snake.length    = 3;

    // Build the rest of the snake behind the head
    for (int i = 0; i < game.snake.length; ++i) {
        Point p = head;
        switch (dir) {
            case 0: p.y += i; break;   // body goes down
            case 1: p.y -= i; break;   // up
            case 2: p.x += i; break;   // right
            case 3: p.x -= i; break;   // left
        }
        game.snake.segments[i] = p;
        game.tiles[p.y][p.x]   = TILE_SNAKE;
    }

    place_food(); // Drop the first food
    draw_score(); // Draw the score                 
}

// Handles snake movement and game logic for each frame
static int move_snake(void)
{
    Point head = game.snake.segments[0];
    Point new_head = head;

    // Move head in current direction
    switch (game.snake.direction) {
        case 0: new_head.y--; break;          // snake goes up
        case 1: new_head.y++; break;          // down
        case 2: new_head.x--; break;          // left 
        case 3: new_head.x++; break;          // right
    }

    // Check for wall collision
    if (new_head.x <= 0 || new_head.x >= SNAKE_BOARD_WIDTH  - 1 ||
        new_head.y <= 0 || new_head.y >= SNAKE_BOARD_HEIGHT - 1){
        if (sound_on) game_sound_fail();
        return 0;
        }
    

    // Check for collision with itself
    for (int i = 0; i < game.snake.length; ++i)
        if (new_head.x == game.snake.segments[i].x &&
            new_head.y == game.snake.segments[i].y){
            if (sound_on) game_sound_fail();
            return 0;  // bit our tail
            } 

    // Save the old tail in case we grow
    Point old_tail = game.snake.segments[ game.snake.length - 1 ];   // save me tail
    
    // Shift all segments forward
    for (int i = game.snake.length - 1; i > 0; --i)
        game.snake.segments[i] = game.snake.segments[i - 1];
    game.snake.segments[0] = new_head;

    // Check for food at new head position
    if (new_head.x == game.food.x && new_head.y == game.food.y) {
        if (game.snake.length < SNAKE_MAX_LENGTH) {
            game.snake.segments[ game.snake.length ] = old_tail;     // Reuse tail as new segment
            ++game.snake.length;                                     
        }
        place_food(); // Drop new food
        if (sound_on)
        {
            game_sound_food();
        }
        draw_score(); // Draw the score again                                      
    }

    return 1; // All good
}

// Draws a line for the menu, green if it’s the selected one
static void menu_line(int x, int y, int idx, int selected, bool sound_on)
{
    // highlight the whole row when selected 
    uint8_t row_col = (selected == idx) ? CLR_GREEN : CLR_NORMAL;

    if (idx != 1) {                                        // normal rows
        const char *txt = (idx == 0) ? "1) Continue"
                          :           "3) Exit to menu";
        game_draw_string(x, y, txt, row_col);
    } else {                                               // sound row
        game_draw_string(x, y, "2) Sound: ", row_col);

        uint8_t onoff_col = sound_on ? CLR_GREEN : CLR_RED;
        const char *state = sound_on ? "ON " : "OFF";
        game_draw_string(x + 10, y, state, onoff_col);
    }
}

// Clears the menu area on the screen
static void clear_menu_area(void)
{
    const int menu_x = VGA_WIDTH - 25;
    for (int y = OFFSET_Y; y < OFFSET_Y + 7; ++y)
        for (int x = menu_x; x < VGA_WIDTH; ++x)
            game_draw_char(x, y, ' ', CLR_NORMAL);
}

// Draws a hint for the pause menu
static void draw_options_hint(void)
{
    const int menu_x = VGA_WIDTH - 25;
    const int menu_y = OFFSET_Y;

    /* erase just the first 25 columns so nothing lingers */
    for (int x = menu_x; x < VGA_WIDTH; ++x)
        game_draw_char(x, menu_y, ' ', CLR_NORMAL);

    game_draw_string(menu_x, menu_y,
                     "Press P for pause options", CLR_NORMAL);
}

// returns 0-Continue, 2-ExitToMenu; toggles sound in-place
static int pause_menu(void)
{
    clear_menu_area();
    const int menu_x = VGA_WIDTH - 20;
    const int menu_y = OFFSET_Y;
    int selected = 0;

    while (1) {
        // Draws the menu
        game_draw_string(menu_x, menu_y, "== PAUSED ==", CLR_NORMAL);
        menu_line(menu_x, menu_y + 2, 0, selected, sound_on);
        menu_line(menu_x, menu_y + 3, 1, selected, sound_on);
        menu_line(menu_x, menu_y + 4, 2, selected, sound_on);

        // waits for keyboard press
        last_key = 0;
        while (last_key == 0) asm volatile("hlt");

        if (last_key == 1 && selected > 0)           --selected;   // menu up 
        else if (last_key == 2 && selected < 2)      ++selected;   // menu down
        else if (last_key == 6) {                                  // select
            if (selected == 0) {           // contiue 
                if (sound_on) game_sound_confirm();
                clear_menu_area();
                return 0;
            }
            if (selected == 1) { // Toggle sound, stay in menu 
                sound_on = !sound_on;
                if (sound_on) game_sound_toggle();
            } else { // Exit to main menu 
                if (sound_on) game_sound_confirm();
                clear_menu_area();
                return 2;
            }
        } else if (last_key == 9) { // ESC key  
            clear_menu_area();
            return 0;
        }
    }
}



// Shows the game over screen and waits for input to restart or quit
static void show_game_over() {
    const char* msg1 = "GAME OVER!";
    const char* msg2 = "Press ENTER to play again";
    const char* msg3 = "ESC to return to main menu";

    int mid_x = SNAKE_BOARD_WIDTH / 2 - 10;
    int mid_y = SNAKE_BOARD_HEIGHT / 2;

    game_draw_string(OFFSET_X + mid_x, OFFSET_Y + mid_y,     msg1, CLR_RED);
    game_draw_string(OFFSET_X + mid_x, OFFSET_Y + mid_y + 1, msg2, 0x07);
    game_draw_string(OFFSET_X + mid_x, OFFSET_Y + mid_y + 2, msg3, 0x07);
}

// Main loop, runs the game, handles input, moves the snake, and redraws everything
void snake_main(void) {
    if (game_start) {
        game_sound_init();
        game_draw_title();
        if (sound_on)
        {
            game_sound_opening(); 
        }
        game_start = false; // only play once
    }
    game_clear_screen();
    draw_board();
    draw_options_hint(); 
    init_snake();
    draw_snake();


    last_key = 0;
    while (1) {
        // Pause menu trigger
        if (last_key == 5) {// P key
            int res = pause_menu();
            if (res == 2) {
                game_start = true; // reset the game start flag
                return;
            }; // exit to main menu
            // resume on return 0: redraw everything
            draw_board();
            draw_food(); 
            draw_snake();
            draw_score();
            draw_options_hint(); 
            last_key = 0;
        }
        clear_snake();

        if (!move_snake()) {
            show_game_over();
            while (1) {
                if (last_key == 6) { // ENTER
                    snake_main(); // Restart game
                    return;
                } else if (last_key == 9) {
                    game_start = true; // reset the game start flag
                    return; // Exit to menu
                }
                sleep_busy(100);
            }
        }

        draw_snake();

        // Change direction based on last_key
        if (last_key >= 1 && last_key <= 4) {
            game.snake.direction = last_key - 1; // match 1-4 to 0-3
        }

        last_key = 0;
        sleep_busy(200); // Control speed of snake
    }
}