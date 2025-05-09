#include "text_editor.h"
#include "drivers/VGA/vga.h"
#include "arch/i386/GDT/util.h"
#include "libc/string.h"
#include "drivers/PIT/pit.h"
#include "menu.h"

extern MenuState current_state;
extern uint8_t last_scancode;

uint16_t cursor_x = 0;
uint16_t cursor_y = 0;
char text_buffer[MAX_BUFFER_SIZE];
uint16_t buffer_size = 0;
char* lines[MAX_LINES];
uint16_t line_count = 0;
uint16_t top_line = 0;

bool editor_modified = false;
char filename[MAX_LINE_LENGTH] = "Untitled.txt";

static uint8_t previous_scancode = 0;

void draw_editor_frame() {
    Reset();
    
    setColor(EDITOR_TITLE_COLOR, EDITOR_BG_COLOR);
    setCursorPosition(0, 0);
    
    for (int i = 0; i < EDITOR_WIDTH; i++) {
        putCharAt(i, 0, '-', EDITOR_BORDER_COLOR, EDITOR_BG_COLOR);
    }
    
    char title[50] = "UiA Text Editor - ";
    strcat(title, filename);
    if (editor_modified) {
        strcat(title, " *");
    }
    
    int title_pos = (EDITOR_WIDTH - strlen(title)) / 2;
    setCursorPosition(title_pos, 0);
    setColor(EDITOR_TITLE_COLOR, EDITOR_BG_COLOR);
    print(title);
    
    for (int i = 1; i < EDITOR_HEIGHT; i++) {
        putCharAt(0, i, '|', EDITOR_BORDER_COLOR, EDITOR_BG_COLOR);
        putCharAt(EDITOR_WIDTH - 1, i, '|', EDITOR_BORDER_COLOR, EDITOR_BG_COLOR);
    }
    
    for (int i = 0; i < EDITOR_WIDTH; i++) {
        putCharAt(i, EDITOR_HEIGHT, '-', EDITOR_BORDER_COLOR, EDITOR_BG_COLOR);
    }
    
    update_status_line();
    
    redraw_text();
}

void update_status_line() {
    char status[80];
    sprintf(status, "Line: %d/%d  Col: %d  %s", 
            cursor_y + 1 + top_line, 
            line_count, 
            cursor_x + 1,
            editor_modified ? "[Modified]" : "[Saved]");
    
    setColor(EDITOR_STATUS_COLOR, EDITOR_BG_COLOR);
    setCursorPosition(2, EDITOR_HEIGHT - 1);
    
    for (int i = 2; i < EDITOR_WIDTH - 2; i++) {
        putCharAt(i, EDITOR_HEIGHT - 1, ' ', EDITOR_STATUS_COLOR, EDITOR_BG_COLOR);
    }
    
    print(status);
    
    setColor(EDITOR_TEXT_COLOR, EDITOR_BG_COLOR);
    setCursorPosition(EDITOR_WIDTH - 30, EDITOR_HEIGHT - 1);
    print("ESC: Exit  Ctrl+S: Save");
}

void update_cursor_position() {
    if (cursor_x < 0) cursor_x = 0;
    if (cursor_y < 0) cursor_y = 0;
    if (cursor_x >= MAX_LINE_LENGTH - 1) cursor_x = MAX_LINE_LENGTH - 1;
    if (cursor_y + top_line >= line_count) {
        if (line_count > 0) {
            cursor_y = line_count - 1 - top_line;
        } else {
            cursor_y = 0;
        }
    }
    
    setCursorPosition(cursor_x + 1, cursor_y + 1);
}

void parse_buffer() {
    line_count = 0;
    
    if (buffer_size == 0) {
        text_buffer[0] = '\0';
        lines[0] = text_buffer;
        line_count = 1;
        return;
    }
    
    char* line_start = text_buffer;
    lines[line_count++] = line_start;
    
    for (uint16_t i = 0; i < buffer_size; i++) {
        if (text_buffer[i] == '\n') {
            text_buffer[i] = '\0';
            if (i + 1 < buffer_size) {
                line_start = &text_buffer[i + 1];
                lines[line_count++] = line_start;
            }
        }
    }
    
    if (buffer_size > 0 && text_buffer[buffer_size - 1] != '\0') {
        lines[line_count++] = &text_buffer[buffer_size];
        text_buffer[buffer_size] = '\0';
    }
}

void redraw_text() {
    setColor(EDITOR_TEXT_COLOR, EDITOR_BG_COLOR);
    
    uint16_t visible_lines = EDITOR_HEIGHT - 2;
    
    for (uint16_t y = 0; y < visible_lines; y++) {
        setCursorPosition(1, y + 1);
        for (uint16_t x = 0; x < EDITOR_WIDTH - 2; x++) {
            putCharAt(x + 1, y + 1, ' ', EDITOR_TEXT_COLOR, EDITOR_BG_COLOR);
        }
    }
    
    for (uint16_t i = 0; i < visible_lines && i + top_line < line_count; i++) {
        setCursorPosition(1, i + 1);
        print(lines[i + top_line]);
    }
    
    update_cursor_position();
}

void move_cursor(int dx, int dy) {
    cursor_x += dx;
    
    if (cursor_x < 0) {
        cursor_x = 0;
    }
    
    char* current_line = lines[cursor_y + top_line];
    uint16_t line_len = strlen(current_line);
    
    if (cursor_x > line_len) {
        cursor_x = line_len;
    }
    
    if (dy != 0) {
        uint16_t new_y = cursor_y + dy;
        
        if (new_y >= EDITOR_HEIGHT - 2) {
            if (top_line + EDITOR_HEIGHT - 2 < line_count) {
                top_line++;
                cursor_y = EDITOR_HEIGHT - 3;
            } else {
                cursor_y = line_count - top_line - 1;
            }
        } else if (new_y < 0) {
            if (top_line > 0) {
                top_line--;
                cursor_y = 0;
            } else {
                cursor_y = 0;
            }
        } else {
            cursor_y = new_y;
        }
        
        if (cursor_y + top_line < line_count) {
            current_line = lines[cursor_y + top_line];
            line_len = strlen(current_line);
            if (cursor_x > line_len) {
                cursor_x = line_len;
            }
        }
    }
    
    redraw_text();
}

void insert_char(char c) {
    editor_modified = true;
    
    if (buffer_size >= MAX_BUFFER_SIZE - 2) {
        return;
    }
    
    if (c == '\n') {
        uint16_t pos = lines[cursor_y + top_line] - text_buffer + cursor_x;
        
        for (int i = buffer_size; i > pos; i--) {
            text_buffer[i] = text_buffer[i - 1];
        }
        
        text_buffer[pos] = '\n';
        buffer_size++;
        
        parse_buffer();
        
        cursor_x = 0;
        move_cursor(0, 1);
        return;
    }
    
    uint16_t pos = lines[cursor_y + top_line] - text_buffer + cursor_x;
    
    for (int i = buffer_size; i > pos; i--) {
        text_buffer[i] = text_buffer[i - 1];
    }
    
    text_buffer[pos] = c;
    buffer_size++;
    
    parse_buffer();
    cursor_x++;
    redraw_text();
}

void delete_char() {
    uint16_t pos = lines[cursor_y + top_line] - text_buffer + cursor_x;
    
    if (pos >= buffer_size) {
        return;
    }
    
    editor_modified = true;
    
    for (int i = pos; i < buffer_size - 1; i++) {
        text_buffer[i] = text_buffer[i + 1];
    }
    
    buffer_size--;
    
    parse_buffer();
    redraw_text();
}

void backspace() {
    if (cursor_x > 0) {
        cursor_x--;
        delete_char();
    } else if (cursor_y + top_line > 0) {
        uint16_t prev_line_len = strlen(lines[cursor_y + top_line - 1]);
        
        uint16_t pos = lines[cursor_y + top_line] - text_buffer;
        
        pos--;
        
        for (int i = pos; i < buffer_size - 1; i++) {
            text_buffer[i] = text_buffer[i + 1];
        }
        
        buffer_size--;
        editor_modified = true;
        
        parse_buffer();
        
        cursor_x = prev_line_len;
        move_cursor(0, -1);
    }
}

char scancode_to_ascii(uint8_t scancode, bool ctrl_pressed) {
    static const char lowercase[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
        0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
        '*', 0, ' '
    };
    
    static const char uppercase[] = {
        0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
        0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
        0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
        '*', 0, ' '
    };
    
    bool shift_pressed = (inPortB(0x64) & 0x01) != 0;
    
    if (ctrl_pressed) {
        if (scancode >= 0x10 && scancode <= 0x32) {
            return scancode - 0x10 + 1;
        }
        return 0;
    }
    
    if (scancode < sizeof(lowercase) && lowercase[scancode] != 0) {
        return shift_pressed ? uppercase[scancode] : lowercase[scancode];
    }
    
    return 0;
}

void handle_editor_input() {
    uint8_t scancode = inPortB(0x60);
    
    bool key_released = scancode & 0x80;
    
    uint8_t clean_scancode = scancode & 0x7F;
    
    if (!key_released && clean_scancode != previous_scancode) {
        previous_scancode = clean_scancode;
        
        bool ctrl_pressed = (inPortB(0x64) & 0x04) != 0;
        
        if (ctrl_pressed && clean_scancode == 0x1F) {
            editor_modified = false;
            update_status_line();
            return;
        }
        
        switch (clean_scancode) {
            case 0x01:
                current_state = MENU_STATE_MAIN;
                break;
                
            case 0x1C:
                insert_char('\n');
                break;
                
            case 0x0E:
                backspace();
                break;
                
            case 0x53:
                delete_char();
                break;
                
            case 0x4B:
                move_cursor(-1, 0);
                break;
                
            case 0x4D:
                move_cursor(1, 0);
                break;
                
            case 0x48:
                move_cursor(0, -1);
                break;
                
            case 0x50:
                move_cursor(0, 1);
                break;
                
            case 0x49:
                move_cursor(0, -10);
                break;
                
            case 0x51:
                move_cursor(0, 10);
                break;
                
            case 0x47:
                cursor_x = 0;
                update_cursor_position();
                break;
                
            case 0x4F:
                cursor_x = strlen(lines[cursor_y + top_line]);
                update_cursor_position();
                break;
                
            default:
                char ascii = scancode_to_ascii(clean_scancode, ctrl_pressed);
                if (ascii >= 32 && ascii <= 126) {
                    insert_char(ascii);
                }
                break;
        }
    }
    else if (key_released && (clean_scancode == previous_scancode)) {
        previous_scancode = 0;
    }
}

void init_text_editor() {
    cursor_x = 0;
    cursor_y = 0;
    buffer_size = 0;
    line_count = 0;
    top_line = 0;
    editor_modified = false;
    strcpy(filename, "Untitled.txt");
    
    previous_scancode = 0;
    
    text_buffer[0] = '\0';
    lines[0] = text_buffer;
    line_count = 1;
    
    draw_editor_frame();
}

void text_editor_loop() {
    handle_editor_input();
    
    update_cursor_position();
    
    sleep_interrupt(1);
}