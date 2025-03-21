#include <libc/stdint.h>

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

// Add to kprint.c
void kprint_hex(uint32_t num) {
    const char hex_chars[] = "0123456789ABCDEF";
    char buffer[11] = "0x00000000";
    
    for (int i = 9; i >= 2; i--) {
        buffer[i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    
    kprint(buffer);
}