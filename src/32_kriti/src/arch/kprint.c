#include <stdint.h>

#define VIDEO_MEMORY 0xB8000
#define VGA_WIDTH 80

static volatile char *video = (volatile char*)VIDEO_MEMORY;
static int cursor_pos = 0;

void kprint(const char *str) {
    while (*str) {
        if (*str == '\n') {
            cursor_pos += VGA_WIDTH - (cursor_pos % VGA_WIDTH); // Move to new line
        } else {
            video[cursor_pos * 2] = *str;       // Character
            video[cursor_pos * 2 + 1] = 0x07;  // White text on black background
            cursor_pos++;
        }
        str++;
    }
}
