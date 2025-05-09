#include "snake.h"
#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "display.h"
#include "interruptHandler.h"
#include "programmableIntervalTimer.h"
#include "pcSpeaker.h"
#include "memory_manager.h"
#include "miscFuncs.h"

// Direkte tilgang til VGA minne
static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

// Skjermdimensjoner
static const int SCREEN_WIDTH = 80;  // Samme som VGA_WIDTH i display.c
static const int SCREEN_HEIGHT = 25;
static const int VGA_WIDTH = 80;     // Brukes med VGA_MEMORY indeksering

// Spillområdedimensjoner (ekskludert rammer)
static const int GAME_WIDTH = 78;
static const int GAME_HEIGHT = 23;

// Startposisjon for slangen
static const int INITIAL_X = 40;
static const int INITIAL_Y = 12;

// Tilfeldig tall frø
static uint32_t random_seed = 12345;

// Enkel implementasjon av strengfunksjoner
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

// Oppretter en VGA oppføring med gitt tegn og farge (kopiert fra display.c)
static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

// Oppretter en VGA fargebyte fra forgrunns- og bakgrunnsfarger (kopiert fra display.c)
static inline uint8_t vga_color(vga_color_t fg, vga_color_t bg) {
    return fg | (bg << 4);
}

// Genererer et enkelt tilfeldig tall
static uint32_t rand(void) {
    random_seed = random_seed * 1103515245 + 12345;
    return (random_seed / 65536) % 32768;
}

// Initialiserer slangespillet
void snake_init(void) {
    // Nullstiller tilfeldig tall frø ved å bruke timertikk
    random_seed = get_current_tick();
}

// Viser slangespillets meny/instruksjoner
void show_snake_menu(void) {
    display_clear();
    display_write_color("\n\n", COLOR_WHITE);
    display_write_color("                           SNAKE GAME\n\n", COLOR_LIGHT_GREEN);
    display_write_color("                     Control with WASD keys:\n\n", COLOR_YELLOW);
    display_write_color("                           W = Up\n", COLOR_WHITE);
    display_write_color("                     A = Left    D = Right\n", COLOR_WHITE);
    display_write_color("                           S = Down\n\n", COLOR_WHITE);
    display_write_color("                    Press ESC to exit the game\n\n", COLOR_WHITE);
    display_write_color("                Press any key to start the game...\n", COLOR_LIGHT_CYAN);
    while (!keyboard_data_available()) {
        __asm__ volatile("hlt");
    }
    keyboard_getchar();
    display_clear();
}

// Initialiserer spilltilstanden
static void init_game_state(GameState* state) {
    // Setter startposisjon for slangen
    state->snake.length = 3;
    state->snake.is_alive = true;
    state->snake.direction = RIGHT;
    
    // Starter i midten av spilleområdet
    int start_x = SCREEN_WIDTH / 2;
    int start_y = 12; // Midten av det nye spilleområdet (mellom linje 1 og 23)
    
    // Oppretter slangesegmenter (hode ved 0)
    for (int i = 0; i < state->snake.length; i++) {
        state->snake.segments[i].x = start_x - i;
        state->snake.segments[i].y = start_y;
    }
    
    // Genererer eple
    generate_apple(state);
    
    // Setter spillhastighet (høyere = saktere)
    state->game_speed = 100;
    
    // Initialiserer poengsum og spilltilstand
    state->score = 0;
    state->game_over = false;
}

// Genererer en ny epleposisjon
void generate_apple(GameState* state) {
    bool valid_position = false;
    
    while (!valid_position) {
        // Genererer en tilfeldig posisjon innenfor spillområdet - justert for nye veggposisjoner
        state->apple.x = 1 + (rand() % (GAME_WIDTH - 2));
        state->apple.y = 2 + (rand() % 20); // Mellom linje 2 og 22
        
        // Sjekker om posisjonen overlapper med slangen
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

// Tegner spillskjermen
void draw_game_screen(const GameState* state) {
    display_clear();
    
    // Øvre vegg - flyttet opp til linje 1
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        display_set_cursor(x, 1);
        display_write_char_color('-', COLOR_LIGHT_CYAN);
    }
    
    // Nedre vegg - flyttet ned til linje 23
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        display_set_cursor(x, 23);
        display_write_char_color('-', COLOR_LIGHT_CYAN);
    }
    
    // Sidevegger
    for (int y = 2; y < 23; y++) {
        display_set_cursor(0, y);
        display_write_char_color('|', COLOR_LIGHT_CYAN);
        display_set_cursor(SCREEN_WIDTH - 1, y);
        display_write_char_color('|', COLOR_LIGHT_CYAN);
    }
    
    // Hjørner
    display_set_cursor(0, 1);
    display_write_char_color('+', COLOR_LIGHT_CYAN);
    display_set_cursor(SCREEN_WIDTH - 1, 1);
    display_write_char_color('+', COLOR_LIGHT_CYAN);
    display_set_cursor(0, 23);
    display_write_char_color('+', COLOR_LIGHT_CYAN);
    display_set_cursor(SCREEN_WIDTH - 1, 23);
    display_write_char_color('+', COLOR_LIGHT_CYAN);
    
    // Resten av funksjonen forblir uendret
    for (int i = 0; i < state->snake.length; i++) {
        int x = state->snake.segments[i].x;
        int y = state->snake.segments[i].y;
        if (x > 0 && x < SCREEN_WIDTH - 1 && y > 1 && y < 23) {
            display_set_cursor(x, y);
            if (i == 0) {
                display_write_char_color(SNAKE_HEAD_CHAR, COLOR_LIGHT_GREEN);
            } else {
                display_write_char_color(SNAKE_CHAR, COLOR_GREEN);
            }
        }
    }
    
    // Tegn eplet hvis det er innenfor spillområdet
    if (state->apple.x > 0 && state->apple.x < SCREEN_WIDTH - 1 && 
        state->apple.y > 1 && state->apple.y < 23) {
        display_set_cursor(state->apple.x, state->apple.y);
        display_write_char_color(APPLE_CHAR, COLOR_LIGHT_RED);
    }
    
    // Vis poengsum
    display_set_cursor(2, SCREEN_HEIGHT);
    display_write_color("Score: ", COLOR_YELLOW);
    char score_text[16];
    int_to_string(state->score, score_text);
    display_write_color(score_text, COLOR_YELLOW);
    
    if (state->game_over) {
        const char* game_over_text = "GAME OVER!";
        int text_len = my_strlen(game_over_text);
        int center_x = (SCREEN_WIDTH - text_len) / 2;
        // Midten av det nye spilleområdet (mellom linje 1 og 23)
        int center_y = 12;
        display_set_cursor(center_x, center_y);
        display_write_color(game_over_text, COLOR_LIGHT_RED);
        const char* restart_text = "Press 'R' to restart or ESC to quit";
        int restart_len = my_strlen(restart_text);
        center_x = (SCREEN_WIDTH - restart_len) / 2;
        display_set_cursor(center_x, center_y + 2);
        display_write_color(restart_text, COLOR_LIGHT_CYAN);
    }
    display_hide_cursor();
}

// Behandler tastaturinndata - forenklet og optimalisert
Direction process_input(Direction current_direction, bool* quit_game) {
    Direction new_direction = current_direction;
    *quit_game = false;
    
    // Sjekker bare tastaturbufferen - mer effektivt
    while (keyboard_data_available()) {
        char key = keyboard_getchar();
        
        // Håndterer escape-tast
        if (key == 27) {
            *quit_game = true;
            return current_direction;
        }
        
        // Håndterer bevegelsesretninger - forenklet logikk
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

// Sjekker for kollisjoner
bool check_collision(const GameState* state) {
    // Henter slangehodets posisjon
    int head_x = state->snake.segments[0].x;
    int head_y = state->snake.segments[0].y;
    
    // Sjekker kollisjon med vegger - oppdatert til å matche nye veggposisjoner
    if (head_x <= 0 || head_x >= SCREEN_WIDTH - 1 || 
        head_y <= 1 || head_y >= 23) {
        return true;
    }
    
    // Sjekker kollisjon med slangekroppen
    for (int i = 1; i < state->snake.length; i++) {
        if (head_x == state->snake.segments[i].x && 
            head_y == state->snake.segments[i].y) {
            return true;
        }
    }
    
    return false;
}

// Flytter slangen
void move_snake(GameState* state) {
    // Lagrer gamle posisjoner for å oppdatere kroppen
    int prev_x = state->snake.segments[0].x;
    int prev_y = state->snake.segments[0].y;
    
    // Flytter hodet basert på retning
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
    
    // Oppdaterer kroppens posisjoner
    for (int i = 1; i < state->snake.length; i++) {
        int temp_x = state->snake.segments[i].x;
        int temp_y = state->snake.segments[i].y;
        
        state->snake.segments[i].x = prev_x;
        state->snake.segments[i].y = prev_y;
        
        prev_x = temp_x;
        prev_y = temp_y;
    }
}

// Oppdaterer spillet
void update_game(GameState* state, Direction input_direction) {
    // Oppdaterer slangehodeets retning
    state->snake.direction = input_direction;
    
    // Flytter slangen
    move_snake(state);
    
    // Sjekker for kollisjoner
    if (check_collision(state)) {
        state->game_over = true;
        return;
    }
    
    // Sjekker om slangen spiser eplet
    if (state->snake.segments[0].x == state->apple.x && 
        state->snake.segments[0].y == state->apple.y) {
        // Øker slangelengden
        state->snake.length++;
        
        // Setter nytt segment til samme posisjon som det siste
        state->snake.segments[state->snake.length - 1].x = 
            state->snake.segments[state->snake.length - 2].x;
        state->snake.segments[state->snake.length - 1].y = 
            state->snake.segments[state->snake.length - 2].y;
        
        // Genererer nytt eple
        generate_apple(state);
        
        // Øker poengsummen
        state->score += 10;
        
        // Øker hastigheten hver 50 poeng
        if (state->score % 50 == 0 && state->game_speed > 20) {
            state->game_speed -= 5;
        }
    }
}

// Håndterer slangespillet
void handle_snake_game(void) {
    // Initialiserer spillet
    snake_init();
    show_snake_menu();
    
    // Oppretter spilltilstand
    GameState state;
    init_game_state(&state);
    
    // Hovedspillløkke
    bool quit_game = false;
    while (!quit_game) {
        // Tegner spillskjermen
        draw_game_screen(&state);
        
        // Behandler tastaturinndata
        Direction input_direction = process_input(state.snake.direction, &quit_game);
        
        // Oppdaterer spillet
        update_game(&state, input_direction);
        
        // Vent litt for å kontrollere spillhastigheten
        sleep_interrupt(state.game_speed);
        
        // Håndterer spillet over tilstand
        if (state.game_over) {
            draw_game_screen(&state);
            
            // Vent på brukerinndata
            while (!keyboard_data_available()) {
                __asm__ volatile("hlt");
            }
            
            char key = keyboard_getchar();
            if (key == 'r' || key == 'R') {
                // Start spillet på nytt
                init_game_state(&state);
            } else if (key == 27) { // ESC
                quit_game = true;
            }
        }
    }
}

// Wrapper for menyen
void snake_game(void) {
    handle_snake_game();
} 