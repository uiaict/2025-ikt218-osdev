// monitor.c -- Defines functions for writing to the monitor.
//             heavily based on Bran's kernel development tutorials,
//             but rewritten for JamesM's kernel tutorials.

#include "monitor.h"
#include "libc/system.h"
#include "common.h"
#include "libc/stdarg.h"

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
uint16_t *video_memory = (uint16_t *)0xB8000;
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

// Scrolls the text on the screen up by one line.
static void scroll()
{
    uint8_t attributeByte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attributeByte << 8);
    if(terminal_row >= 25)
    {
        for (int i = 0*80; i < 24*80; i++)
        {
            terminal_buffer[i] = terminal_buffer[i+80];
        }
        for (int i = 24*80; i < 25*80; i++)
        {
            terminal_buffer[i] = blank;
        }
        terminal_row = 24;
    }
}



static void move_cursor()
{
    uint16_t pos = terminal_row * 80 + terminal_column;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

void monitor_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = video_memory;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}
 
void monitor_backspace() {
    if (terminal_column > 0) {
        terminal_column--;
        monitor_putentryat(' ', terminal_color, terminal_column, terminal_row);
        move_cursor();
    }
}

void monitor_setcolor(uint8_t color) 
{
	terminal_color = color;
}
 
void monitor_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void _monitor_put(char c) 
{
	switch (c)
	{
	case '\n':
		terminal_column = 0;
		terminal_row++;
        scroll();
		return;
	default:
		break;
	}
	monitor_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void monitor_put(char c) 
{
	_monitor_put(c);
    scroll();
    move_cursor();
}
 
void monitor_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		_monitor_put(data[i]);
    scroll();
    move_cursor();
}
 
void monitor_writestring(const char* data) 
{
	monitor_write(data, strlen(data));
}

void monitor_clear()
{
    uint8_t attributeByte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attributeByte << 8);
    for (int i = 0; i < 80*25; i++)
    {
        terminal_buffer[i] = blank;
    }
    terminal_row = 0;
    terminal_column = 0;
    move_cursor();
}

void monitor_write_hex(uint32_t n)
{
    int32_t tmp;
    char* item = "0x";
    monitor_write(item, strlen(item));
    char noZeroes = 1;
    for (int i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            monitor_put (tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
            monitor_put( tmp+'0' );
        }
    }
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        monitor_put (tmp-0xA+'a');
    }
    else
    {
        monitor_put (tmp+'0');
    }
}

void monitor_write_dec(uint32_t n)
{
    if (n == 0)
    {
        monitor_put('0');
        return;
    }
    int32_t acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc % 10;
        acc /= 10;
        i++;
    }
    c[i] = 0;
    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    monitor_write(c2, strlen(c2));
}

// Simple implementation of vsnprintf for kernel use
int vsnprintf(char* str, size_t size, const char* format, va_list args) {
    int written = 0;
    size_t i;
    
    for (i = 0; format[i] != '\0' && (written < size - 1); i++) {
        if (format[i] != '%') {
            str[written++] = format[i];
            continue;
        }
        
        // Handle format specifiers
        i++; // Skip the '%'
        
        switch (format[i]) {
            case 'd': 
            case 'i': {
                int val = va_arg(args, int);
                char num_buffer[12]; // Enough for 32-bit integer
                int len = 0;
                int is_negative = 0;
                
                if (val < 0) {
                    is_negative = 1;
                    val = -val;
                }
                
                // Convert to string (reversed)
                do {
                    num_buffer[len++] = '0' + (val % 10);
                    val /= 10;
                } while (val && len < sizeof(num_buffer) - 1);
                
                // Add negative sign if needed
                if (is_negative && len < sizeof(num_buffer) - 1) {
                    num_buffer[len++] = '-';
                }
                
                // Copy reversed string to output
                while (len > 0 && written < size - 1) {
                    str[written++] = num_buffer[--len];
                }
                break;
            }
            
            case 'u': {
                unsigned int val = va_arg(args, unsigned int);
                char num_buffer[11]; // Enough for 32-bit unsigned
                int len = 0;
                
                // Convert to string (reversed)
                do {
                    num_buffer[len++] = '0' + (val % 10);
                    val /= 10;
                } while (val && len < sizeof(num_buffer) - 1);
                
                // Copy reversed string to output
                while (len > 0 && written < size - 1) {
                    str[written++] = num_buffer[--len];
                }
                break;
            }
            
            case 'x': 
            case 'X': {
                unsigned int val = va_arg(args, unsigned int);
                char num_buffer[9]; // Enough for 32-bit hex
                int len = 0;
                const char* hex_chars = (format[i] == 'x') ? "0123456789abcdef" : "0123456789ABCDEF";
                
                // Convert to hex string (reversed)
                do {
                    num_buffer[len++] = hex_chars[val & 0xF];
                    val >>= 4;
                } while (val && len < sizeof(num_buffer) - 1);
                
                // Copy reversed string to output
                while (len > 0 && written < size - 1) {
                    str[written++] = num_buffer[--len];
                }
                break;
            }
            
            case 'c': {
                char val = (char)va_arg(args, int);
                str[written++] = val;
                break;
            }
            
            case 's': {
                const char* val = va_arg(args, const char*);
                if (!val) val = "(null)";
                
                while (*val && written < size - 1) {
                    str[written++] = *val++;
                }
                break;
            }
            
            case '%': {
                str[written++] = '%';
                break;
            }
            
            default:
                // Unsupported format specifier
                str[written++] = '%';
                if (written < size - 1)
                    str[written++] = format[i];
                break;
        }
    }
    
    // Null-terminate the string
    str[written] = '\0';
    
    return written;
}

void terminal_printf(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    monitor_writestring(buffer);
}

void terminal_clear(void) {
    monitor_clear();
}