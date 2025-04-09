#include "state/shell_command.h"
#include "terminal/cursor.h"
#include "terminal/print.h"
#include "memory/heap.h"

#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "libc/stdint.h"

#define SCREEN_WIDTH 80
#define LAST_ROW 24
#define VGA_ADDRESS 0xB8000

static char command_buffer[SCREEN_WIDTH];

static void string_shorten() {
    int i = SCREEN_WIDTH - 1;
    while (i >= 0 && (command_buffer[i] == ' ' || command_buffer[i] == '\r')) {
        command_buffer[i] = '\0';
        i--;
    }
}

void get_last_line() {
    memset(command_buffer, 0, SCREEN_WIDTH);

    uint16_t *video_memory = (uint16_t*) VGA_ADDRESS;
    int row;
    int line_start;
    
    if (cursor_position == 0) {
        row = LAST_ROW;
    } else {
        row = (cursor_position / SCREEN_WIDTH) - 1;   
    }
    line_start = row * SCREEN_WIDTH;
    
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        uint16_t entry = video_memory[line_start + i];
        char c = (char)(entry & 0xFF);
        command_buffer[i] = c;
    }
    string_shorten();
    
}
