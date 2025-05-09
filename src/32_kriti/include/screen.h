// screen.h
#ifndef SCREEN_H
#define SCREEN_H

// Screen dimensions (standard VGA text mode)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// Color definitions
#define COLOR_BLACK     0x0
#define COLOR_BLUE      0x1
#define COLOR_GREEN     0x2
#define COLOR_CYAN      0x3
#define COLOR_RED       0x4
#define COLOR_MAGENTA   0x5
#define COLOR_BROWN     0x6
#define COLOR_LGRAY     0x7
#define COLOR_DGRAY     0x8
#define COLOR_LBLUE     0x9
#define COLOR_LGREEN    0xA
#define COLOR_LCYAN     0xB
#define COLOR_LRED      0xC
#define COLOR_LMAGENTA  0xD
#define COLOR_YELLOW    0xE
#define COLOR_WHITE     0xF

// VGA text buffer address
#define VGA_TEXT_BUFFER 0xB8000

// Functions for screen manipulation
void clear_screen(void);
void set_cursor_pos(int x, int y);
void print_char(char c);
void print_string(const char *str);
void print_int(int n);
void set_text_color(unsigned char foreground, unsigned char background);
void scroll_screen(void);
void init_screen(void);

#endif