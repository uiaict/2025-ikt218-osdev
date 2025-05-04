# ifndef TERMINAL_H
# define TERMINAL_H
# include <libc/stdint.h>

extern uint16_t cursor_pos;
extern uint16_t *vga_buffer;

void move_cursor(uint16_t position);

void clear_screen();
void print_string(const char *str);
void print_int(int num);
void print_char(char c);
void printf(const char *format, ...);
void print_hex(uint32_t num);
void scroll_down();


# endif