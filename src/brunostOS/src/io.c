#include "io.h"
#include "memory/memutils.h"


uint8_t terminal_color = 0x0F; // default white text black background
int cursor_xpos = 0;
int cursor_ypos = 0;
char *video_memory = (char*)0x00B8000;

void set_vga_color(enum vga_color txt_color, enum vga_color bg_color){
    terminal_color = txt_color | bg_color << 4;
}
enum vga_color get_vga_txt_clr(){
    return (enum vga_color)(terminal_color & 0x0F);
}
enum vga_color get_vga_bg_clr(){
    return (enum vga_color)((terminal_color >> 4) & 0x0F);
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end){
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}
void disable_cursor(){
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}
void update_cursor(){
    uint16_t pos = cursor_ypos * VGA_WIDTH + cursor_xpos;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
} 



void outb(uint16_t port, uint8_t val){
    asm volatile ( "outb %b0, %w1"
                    :
                    : "a"(val), "Nd"(port) 
                    : "memory");
};

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

void clear_terminal(){
    for (size_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++){
        video_memory[i*2] = 0;
        video_memory[i*2 +1] = 0x0F;
    }
}

void reset_cursor_pos(){
    cursor_xpos = 0;
    cursor_ypos = 0;
}