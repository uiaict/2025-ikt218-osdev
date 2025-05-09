#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "drivers/VGA/vga.h"
#include "menu.h"

#define EDITOR_WIDTH 78
#define EDITOR_HEIGHT 20
#define EDITOR_BORDER_COLOR COLOR8_LIGHT_BLUE
#define EDITOR_TITLE_COLOR COLOR8_CYAN
#define EDITOR_TEXT_COLOR COLOR8_WHITE
#define EDITOR_HIGHLIGHT_COLOR COLOR8_YELLOW
#define EDITOR_BG_COLOR COLOR8_BLACK
#define EDITOR_STATUS_COLOR COLOR8_GREEN

#define MAX_BUFFER_SIZE 2000
#define MAX_LINES 100
#define MAX_LINE_LENGTH 80

void init_text_editor();
void text_editor_loop();
void draw_editor_frame();
void handle_editor_input();
void update_editor();
void insert_char(char c);
void delete_char();
void move_cursor(int dx, int dy);
void redraw_text();
void update_status_line();
char scancode_to_ascii(uint8_t scancode, bool ctrl_pressed);

extern uint16_t cursor_x;
extern uint16_t cursor_y;
extern char text_buffer[MAX_BUFFER_SIZE];
extern uint16_t buffer_size;
extern char* lines[MAX_LINES];
extern uint16_t line_count;
extern uint16_t top_line;

extern bool editor_modified;
extern char filename[MAX_LINE_LENGTH];

#endif // TEXT_EDITOR_H