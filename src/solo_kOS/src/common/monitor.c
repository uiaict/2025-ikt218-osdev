// monitor.c
#include "common/monitor.h"
#include "kernel/memory.h"

// First we define the size of the VGA text mode screen
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// scroll buffer logic so we can scroll up and down
// This is the number of lines we can scroll
#define SCROLL_BUFFER_LINES 200

// Software scroll buffer (100 lines)
static char scroll_buffer[SCROLL_BUFFER_LINES][VGA_WIDTH];
static uint8_t scroll_colors[SCROLL_BUFFER_LINES][VGA_WIDTH];

static int scroll_total_lines = 0;  // How many total lines have been written
static int scroll_top_line = 0;     // First visible line in the buffer
static int buffer_cursor_row = 0;   // Logical row inside the buffer
static int buffer_cursor_col = 0;   // Logical column inside the buffer


// This is the global monitor instance, kept static so it’s private to this file
static monitor_t monitor;

// This function renders the current state of the scroll buffer to the VGA memory
void monitor_render() {
    for (int row = 0; row < VGA_HEIGHT; row++) {
        int buf_row = scroll_top_line + row;
        if (buf_row >= SCROLL_BUFFER_LINES) break;

        for (int col = 0; col < VGA_WIDTH; col++) {
            size_t index = (row * VGA_WIDTH + col) * 2;
            monitor.video_memory[index] = scroll_buffer[buf_row][col];
            monitor.video_memory[index + 1] = scroll_colors[buf_row][col];
        }
    }

    // Update hardware cursor position
    monitor.cursor_row = buffer_cursor_row - scroll_top_line;
    monitor.cursor_col = buffer_cursor_col;
}

/*
Initializes the monitor state:
    - Sets cursor to top-left (row 0, col 0)
    - Points video_memory to the start of VGA memory
    - Sets the default color to light grey on black (0x07)
*/
void monitor_init() {
    monitor.cursor_row = 0;
    monitor.cursor_col = 0;
    monitor.video_memory = (volatile char*)0xb8000;
    monitor.color = 0x07; // Light grey on black

    // Clear screen
    for (int row = 0; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            size_t index = (row * VGA_WIDTH + col) * 2;
            monitor.video_memory[index] = ' ';
            monitor.video_memory[index + 1] = monitor.color;
        }
    }

    // Clear the scroll buffer to start clean
    memset(scroll_buffer, ' ', sizeof(scroll_buffer));
    memset(scroll_colors, monitor.color, sizeof(scroll_colors));
    scroll_total_lines = 0;
    scroll_top_line = 0;
    buffer_cursor_row = 0;
    buffer_cursor_col = 0;

    // Initial render of the (empty) buffer
    monitor_render();
}

// This is where we output a character to the monitor
void monitor_put_char(char c) {
    if (c == '\n') {
        buffer_cursor_col = 0;
        buffer_cursor_row++;

        if (buffer_cursor_row >= SCROLL_BUFFER_LINES) {
            buffer_cursor_row = SCROLL_BUFFER_LINES - 1;
            // Optional: Scroll buffer up if full
            for (int i = 1; i < SCROLL_BUFFER_LINES; i++) {
                memcpy(scroll_buffer[i - 1], scroll_buffer[i], VGA_WIDTH);
                memcpy(scroll_colors[i - 1], scroll_colors[i], VGA_WIDTH);
            }
            memset(scroll_buffer[SCROLL_BUFFER_LINES - 1], ' ', VGA_WIDTH);
            memset(scroll_colors[SCROLL_BUFFER_LINES - 1], monitor.color, VGA_WIDTH);
        }

            // Adjust visible top line if necessary
        if (buffer_cursor_row - scroll_top_line >= VGA_HEIGHT) {
            scroll_top_line = buffer_cursor_row - VGA_HEIGHT + 1;
        }

        scroll_total_lines = buffer_cursor_row + 1;
        monitor_render();
        return;
    }

    scroll_buffer[buffer_cursor_row][buffer_cursor_col] = c;
    scroll_colors[buffer_cursor_row][buffer_cursor_col] = monitor.color;
    buffer_cursor_col++;

    if (buffer_cursor_col >= VGA_WIDTH) {
        buffer_cursor_col = 0;
        buffer_cursor_row++;
        scroll_total_lines = buffer_cursor_row + 1;
    }

    if (buffer_cursor_row >= SCROLL_BUFFER_LINES) {
        buffer_cursor_row = SCROLL_BUFFER_LINES - 1;
    }

    monitor_render();
}

// Writes a null-terminated string to the screen, one character at a time
void monitor_write(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        monitor_put_char(str[i]);
    }
}

// Clears the screen and the scroll buffer, and resets the cursor
void monitor_clear() {
    // Clear the scroll buffer contents
    memset(scroll_buffer, ' ', sizeof(scroll_buffer));
    memset(scroll_colors, monitor.color, sizeof(scroll_colors));

    // Reset all scroll tracking variables
    scroll_top_line = 0;
    scroll_total_lines = 0;
    buffer_cursor_row = 0;
    buffer_cursor_col = 0;

    // Clear the visible VGA memory
    for (int row = 0; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            size_t index = (row * VGA_WIDTH + col) * 2;
            monitor.video_memory[index] = ' ';
            monitor.video_memory[index + 1] = monitor.color;
        }
    }

    monitor.cursor_row = 0;
    monitor.cursor_col = 0;

    // Re-render the now-empty scroll buffer
    monitor_render();
}

void monitor_backspace() {
    if (buffer_cursor_col == 0 && buffer_cursor_row == 0) return;

    if (buffer_cursor_col == 0) {
        buffer_cursor_row--;
        buffer_cursor_col = VGA_WIDTH - 1;
    } else {
        buffer_cursor_col--;
    }

    scroll_buffer[buffer_cursor_row][buffer_cursor_col] = ' ';
    scroll_colors[buffer_cursor_row][buffer_cursor_col] = monitor.color;
    monitor_render();
}

void monitor_enter() {
    monitor.cursor_col = 0;
    monitor.cursor_row++;

    // Prevent cursor from going off screen
    if (monitor.cursor_row >= VGA_HEIGHT) {
        monitor.cursor_row = 0; // Or scroll later
    }
}

void monitor_scroll_up() {
    if (scroll_top_line > 0) {
        scroll_top_line--;
        monitor_render();
    }
}

void monitor_scroll_down() {
    if (scroll_top_line + VGA_HEIGHT < scroll_total_lines) {
        scroll_top_line++;
        monitor_render();
    }
}

void monitor_write_dec(int num) {
    char buffer[16];
    int i = 0;

    if (num == 0) {
        monitor_put_char('0');
        return;
    }

    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    for (int j = i - 1; j >= 0; j--) {
        monitor_put_char(buffer[j]);
    }
}