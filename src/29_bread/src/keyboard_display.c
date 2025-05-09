#include <print.h>
#include <keyboard.h>
#include <libc/stdint.h>

// Track which keys are currently pressed
volatile uint8_t key_states[8] = {0};

// Set key state (1 = pressed, 0 = released)
void set_key_state(uint8_t key_index, uint8_t state) {
    if (key_index < 8) {
        key_states[key_index] = state;
    }
}

// Draw the keyboard at a specific position
void draw_keyboard(uint8_t row, uint8_t col) {
    // Save cursor position
    uint8_t cursor_row, cursor_col;
    terminal_get_cursor(&cursor_row, &cursor_col);
    
    // Draw the keyboard frame
    terminal_goto(row, col);
    printf("┌───┬───┬───┬───┬───┬───┬───┬───┐");
    
    terminal_goto(row + 1, col);
    printf("│ %c │ %c │ %c │ %c │ %c │ %c │ %c │ %c │", 
           key_states[0] ? '#' : '1',
           key_states[1] ? '#' : '2',
           key_states[2] ? '#' : '3',
           key_states[3] ? '#' : '4',
           key_states[4] ? '#' : '5',
           key_states[5] ? '#' : '6',
           key_states[6] ? '#' : '7',
           key_states[7] ? '#' : '8');
    
    terminal_goto(row + 2, col);
    printf("└───┴───┴───┴───┴───┴───┴───┴───┘");
    
    // Note names
    terminal_goto(row + 3, col);
    printf(" C   D   E   F   G   A   B   C ");
    
    // Restore cursor position
    terminal_goto(cursor_row, cursor_col);
}

// Update key state when a key is pressed
void keyboard_display_key_press(char key) {
    if (key >= '1' && key <= '8') {
        uint8_t index = key - '1'; 
        set_key_state(index, 1);  // Key pressed
        draw_keyboard(18, 10);    // Redraw at row 18, column 10
    }
}

// Update key state when a key is released
void keyboard_display_key_release(char key) {
    if (key >= '1' && key <= '8') {
        uint8_t index = key - '1';
        set_key_state(index, 0);  // Key released
        draw_keyboard(18, 10);    // Redraw at row 18, column 10
    }
}