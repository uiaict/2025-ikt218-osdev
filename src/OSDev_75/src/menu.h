#ifndef MENU_H
#define MENU_H

#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "libc/stddef.h"  
#include "drivers/VGA/vga.h"
#include "arch/i386/interrupts/keyboard.h"
#include "drivers/PIT/pit.h"
#include "drivers/audio/song.h"
#include "arch/i386/memory/memory.h"
#include "drivers/VGA/vga_graphics.h"

// Menu constants
#define MENU_WIDTH 60
#define MENU_HEIGHT 15
#define MENU_START_X 10
#define MENU_START_Y 5

// Colors
#define MENU_BORDER_COLOR COLOR8_CYAN
#define MENU_BG_COLOR COLOR8_BLACK
#define MENU_TITLE_COLOR COLOR8_LIGHT_MAGENTA
#define MENU_TEXT_COLOR COLOR8_WHITE
#define MENU_SELECTED_COLOR COLOR8_YELLOW
#define MENU_SELECTED_BG COLOR8_BLUE

#define UIA_RED 4
#define UIA_WHITE 15

// Menu states
typedef enum {
    MENU_STATE_MAIN,
    MENU_STATE_PONG,
    MENU_STATE_MUSIC,
    MENU_STATE_TEXT_EDITOR,
    MENU_STATE_ABOUT,
    MENU_STATE_MUSIC_PLAYER
} MenuState;

// Function prototypes
void init_menu();
void draw_menu_border(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void draw_menu_options(const char** options, uint8_t count, uint8_t selected);
void draw_title_bar(const char* title);
void draw_footer(const char* message);
void draw_menu_shadow();
void animate_menu_open();
void draw_system_info();
void wipe_screen_transition();
void draw_main_menu();
void handle_menu_input();
void menu_loop();
void run_menu();
void show_about();
void fade_transition(uint8_t start_color, uint8_t end_color, uint16_t duration_ms);
void show_uia_splash();

uint32_t os_rand();
void os_srand(uint32_t seed);
uint32_t os_rand_range(uint32_t max);

void matrix_rain_effect();
void enhanced_uia_splash();

extern MenuState current_state;
extern uint8_t selected_option;
extern uint8_t last_scancode;

#endif // MENU_H