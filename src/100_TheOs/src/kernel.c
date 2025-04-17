#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include <libc/stdarg.h> 


extern void init_gdt(void);
extern void start_isr_handlers();
extern void start_keyboard();
extern void display_prompt();

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag first;
};

// Video memory base address
static char* video_memory = (char*)0xB8000;
static int cursor_x = 0;
static int cursor_y = 0;

// Function to set the cursor position
void update_cursor() {
    // Calculate position
    uint16_t pos = cursor_y * 80 + cursor_x;
    
    // Send position to VGA controller
    outb(0x3D4, 14);                // Set high byte
    outb(0x3D5, (pos >> 8) & 0xFF); // Send high byte
    outb(0x3D4, 15);                // Set low byte
    outb(0x3D5, pos & 0xFF);        // Send low byte
}

// Scroll the terminal up one line
void terminal_scroll() {
    // Move all lines up by one
    for(int y = 0; y < 24; y++) {  // 25-1=24 lines to move
        for(int x = 0; x < 80; x++) {
            int current = (y * 80 + x) * 2;
            int next = ((y + 1) * 80 + x) * 2;
            
            video_memory[current] = video_memory[next];
            video_memory[current + 1] = video_memory[next + 1];
        }
    }
    
    // Clear the last line
    for(int x = 0; x < 80; x++) {
        int position = (24 * 80 + x) * 2;
        video_memory[position] = ' ';
        video_memory[position + 1] = 0x0F;  // White on black
    }
}

void terminal_write(const char* str) {
    while (*str) {
        // Handle backspace
        if (*str == '\b') {
            if (cursor_x > 0) {
                cursor_x--;
                // Erase the character at current position
                int offset = (cursor_y * 80 + cursor_x) * 2;
                video_memory[offset] = ' ';
                video_memory[offset + 1] = 0x0F;  // White on black
            }
            str++;
            continue;
        }
        
        // Handle newline
        if (*str == '\n') {
            cursor_x = 0;
            cursor_y++;
           
            // Check if we need to scroll
            if (cursor_y >= 25) {
                terminal_scroll();
                cursor_y = 24;  // Stay on last line
            }
           
            str++;
            continue;
        }
        
        // Check if we need to wrap to next line
        if (cursor_x >= 80) {
            cursor_x = 0;
            cursor_y++;
           
            // Check if we need to scroll
            if (cursor_y >= 25) {
                terminal_scroll();
                cursor_y = 24;  // Stay on last line
            }
        }
        
        // Calculate the offset in video memory
        int offset = (cursor_y * 80 + cursor_x) * 2;
       
        // Write character and color attribute
        video_memory[offset] = *str;
        video_memory[offset + 1] = 0x0F;  // White text on black background
        
        str++;
        cursor_x++;
    }
    update_cursor();
}

// Clear the terminal screen
void terminal_clear() {
    // Fill the entire screen with spaces
    for(int i = 0; i < 80 * 25; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = 0x0F;  // White text on black background
    }
    
    // Reset cursor position
    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

static void itoa(int value, char* str, int base) {
    static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    
    // Handle 0 explicitly
    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    // Handle negative numbers
    int sign = 0;
    if (value < 0 && base == 10) {
        sign = 1;
        value = -value;
    }
    
    // Process digits in reverse
    int i = 0;
    while (value != 0) {
        str[i++] = digits[value % base];
        value /= base;
    }
    
    // Add negative sign if needed
    if (sign) {
        str[i++] = '-';
    }
    
    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
    
    // Null terminate
    str[i] = '\0';
}

// NEW: Simple implementation of printf
void terminal_printf(const char* format, ...) {
    char buffer[256];
    char num_buffer[32];
    char* buf_ptr = buffer;
    
    // Start variadic argument processing
    va_list args;
    va_start(args, format);
    
    // Process the format string
    while (*format != '\0') {
        if (*format != '%') {
            // Normal character - copy to buffer
            *buf_ptr++ = *format++;
            continue;
        }
        
        // Handle format specifier
        format++; // Skip the '%'
        
        switch (*format) {
            case 's': {
                // String
                char* str = va_arg(args, char*);
                while (*str) {
                    *buf_ptr++ = *str++;
                }
                break;
            }
            case 'd':
            case 'i': {
                // Integer
                int value = va_arg(args, int);
                itoa(value, num_buffer, 10);
                char* str = num_buffer;
                while (*str) {
                    *buf_ptr++ = *str++;
                }
                break;
            }
            case 'x': {
                // Hexadecimal
                unsigned int value = va_arg(args, unsigned int);
                itoa(value, num_buffer, 16);
                char* str = num_buffer;
                while (*str) {
                    *buf_ptr++ = *str++;
                }
                break;
            }
            case 'c': {
                // Character
                *buf_ptr++ = (char)va_arg(args, int);
                break;
            }
            case '%': {
                // Literal '%'
                *buf_ptr++ = '%';
                break;
            }
            default: {
                // Unknown format specifier - just output it
                *buf_ptr++ = '%';
                *buf_ptr++ = *format;
                break;
            }
        }
        
        format++;
    }
    
    // Null terminate the buffer and print it
    *buf_ptr = '\0';
    terminal_write(buffer);
    
    // End variadic argument processing
    va_end(args);
}



int main(uint32_t magic, void* mb_info) {
    // Initialize the GDT
    init_gdt();
    
    // Initialize IDT
    start_idt();
    terminal_printf("IDT Initialized\n");
    
    // Initialize IRQ
    start_irq();
    terminal_printf("IRQ Initialized\n");
    
    // Initialize ISR handlers
    start_isr_controllers();
    terminal_printf("ISR Handlers Initialized\n");
    
    // Initialize keyboard
    start_keyboard();
    
    // Enable interrupts globally
    asm volatile("sti");
    
    // Test interrupts
    terminal_printf("Testing interrupts...\n");
    
    // Test division by zero interrupt (INT 0)
    terminal_printf("Testing division by zero interrupt...\n");
    asm volatile("int $0x0");
    
    // Test debug interrupt (INT 1)
    terminal_printf("Testing debug interrupt...\n");
    asm volatile("int $0x1");
    
    // Test NMI interrupt (INT 2)
    terminal_printf("Testing NMI interrupt...\n");
    asm volatile("int $0x2");
    
    terminal_printf("Interrupt testing complete.\n");
    terminal_printf("System is ready. You can start typing...\n");
    display_prompt();
    // Main loop
    while(1) {
        asm volatile("hlt");
    }
    
    return 0;
}
