#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "gdt.h"


#define VGA_ADDRESS 0xB8000
#define VGA_ROWS 25
#define VGA_COLUMS 80

int cursor_x = 0;
int cursor_y = 0;

uint16_t vga_mem_index = 0;
volatile char * vga_buffer = (volatile char*)VGA_ADDRESS;


struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

void printf(int color, const char* str) {
    
    while(*str != '\0') {
        if(cursor_x >=80 || *str == '\n') {
            cursor_x = 0;
            cursor_y++;
        }
        int index = (cursor_y * 80 + cursor_x) * 2;
        vga_buffer[index] = *str; 
        vga_buffer[index+1] = color; 
        str++;
        cursor_x++;
    }
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    initGdt();
    printf(15, "Hello, world!");
    return 0;

}