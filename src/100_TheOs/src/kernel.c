#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag first;
};

// Video memory base address
static char* video_memory = (char*)0xB8000;
static int cursor_x = 0;
static int cursor_y = 0;

// Write a string to the terminal
void terminal_write(const char* str) {
    while (*str) {
        // Check if we need to wrap to next line
        if (cursor_x >= 80) {
            cursor_x = 0;
            cursor_y++;
        }

        // Calculate the offset in video memory
        int offset = (cursor_y * 80 + cursor_x) * 2;
        
        // Write character and color attribute
        video_memory[offset] = *str;
        video_memory[offset + 1] = 0x0F;  // White text on black background

        str++;
        cursor_x++;
    }
}

int main(uint32_t magic, void* mb_info) {
    // Write "Hello World"
    terminal_write("Hello kernel");
    
    // Hang the kernel
    while(1) {}
    
    return 0;
}