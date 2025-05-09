#include "apps/game/snake.h"

void game_clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i * 2] = ' ';
        VGA_MEMORY[i * 2 + 1] = 0x07;  // light grey on black
    }
}

void game_draw_char(int x, int y, char c, uint8_t color) {
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT) return;
    size_t index = (y * VGA_WIDTH + x) * 2;
    VGA_MEMORY[index] = c;
    VGA_MEMORY[index + 1] = color;
}

void game_draw_string(int x, int y, const char* str, uint8_t color) {
    while (*str) {
        game_draw_char(x++, y, *str++, color);
    }
}

void game_draw_title(void) {
    /* 14-line ASCII art banner */
    const char* const title[] = {
    "    _____   __      _ ____   __   ___ _____  ",
    "   / ____\\ /  \\    / |    ) () ) / __) ___/  ",
    "   ( (___  / /\\ \\  / // /\\ \\ ( (_/ / ( (__   ", 
    "    \\___ \\ ) ) ) ) ) | (__) )()   (   ) __)  ", 
    "        ) | ( ( ( ( ( )    ( () /\\ \\ ( (     ", 
    "    ___/ // /  \\ \\/ //  /\\  \\( (  \\ \\ \\ \\___ ", 
    "  /____/(_/    \\__//__(  )__()_)  \\_\\ \\____\\ ",
    "       _____  ____     __    __  _____       ", 
    "      / ___ \\(    )    \\ \\  / / / ___/       ", 
    "     / /   \\_) /\\ \\    () \\/ ()( (__         ", 
    "    ( (  ___( (__) )   / _  _ \\ ) __)        ",
    "    ( ( (__  )    (   / / \\/ \\ ( (           ",
    "     \\ \\__/ /  /\\  \\ /_/      \\_\\ \\___       ",
    "      \\____/__(  )__(/          \\)____\\      ",
    };

    const int lines = sizeof(title) / sizeof(title[0]);

    /* Vertical centring */
    const int start_y = (VGA_HEIGHT - lines) / 2;

    /* Draw each line, centred horizontally */
    for (int i = 0; i < lines; ++i) {
        size_t line_len = strlen(title[i]);
        int start_x = (int)((VGA_WIDTH - line_len) / 2);
        game_draw_string(start_x, start_y + i, title[i], 0x07); // light-grey on black
    }
}
