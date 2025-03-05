#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

// Simple inline GDT flush implementation
void gdt_flush(uint32_t gdt_ptr_addr) {
    asm volatile (
        "movl %0, %%eax\n"
        "lgdt (%%eax)\n"
        "movw $0x10, %%ax\n"
        "movw %%ax, %%ds\n"
        "movw %%ax, %%es\n"
        "movw %%ax, %%fs\n"
        "movw %%ax, %%gs\n"
        "movw %%ax, %%ss\n"
        "ljmp $0x08, $1f\n"
        "1:\n"
        :
        : "r" (gdt_ptr_addr)
        : "eax", "memory"
    );
}

// GDT structures
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// GDT entries
#define GDT_ENTRIES 3
static struct gdt_entry gdt_entries[GDT_ENTRIES];
static struct gdt_ptr gdt_pointer;

// Set up a GDT entry
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt_entries[num].access = access;
}

// Initialize GDT
void gdt_init(void) {
    gdt_pointer.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
    gdt_pointer.base = (uint32_t)&gdt_entries;

    // NULL descriptor
    gdt_set_gate(0, 0, 0, 0, 0);
    
    // Code segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    
    // Data segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    
    // Load GDT
    gdt_flush((uint32_t)&gdt_pointer);
}

// VGA text mode constants
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// VGA color codes
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

// Terminal state
static uint16_t* terminal_buffer;
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;

// Create a VGA entry color byte
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

// Create a VGA character entry
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

// Initialize the terminal
void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK); // RedBull color!
    terminal_buffer = (uint16_t*) VGA_MEMORY;
    
    // Clear the screen
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

// Put a character at a specific position
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

// Change text color
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

// Put a character at the current position
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT)
                terminal_row = 0;
        }
    }
}

// Write a string to the terminal
void terminal_writestring(const char* data) {
    for (size_t i = 0; data[i]; i++)
        terminal_putchar(data[i]);
}

// Simple delay function
void delay(uint32_t count) {
    volatile uint32_t i = 0;
    for (i = 0; i < count; i++) {
        // Just waste some cycles
        __asm__ volatile("nop");
    }
}

// Wing animation frames
const char* wing_frames[] = {
    "    \\_/    ",
    "   _/ \\_   ",
    "  /     \\  ",
    " /       \\ ",
    "/         \\",
    " \\       / ",
    "  \\     /  ",
    "   \\_/\\_   "
};

#define NUM_FRAMES 8

// Display the RedBull OS logo and loading animation
void display_redbull_logo() {
    // Clear screen first
    terminal_initialize();
    
    // Set colors for RedBull theme
    uint8_t title_color = vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
    uint8_t logo_color = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    uint8_t slogan_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    // Display RedBull OS title
    terminal_setcolor(title_color);
    
    // Center the title on row 5
    terminal_row = 5;
    terminal_column = (VGA_WIDTH - 17) / 2;  // "RedBull OS v1.0" is 17 chars
    terminal_writestring("RedBull OS v1.0");
    
    // Display slogan
    terminal_setcolor(slogan_color);
    terminal_row = 7;
    terminal_column = (VGA_WIDTH - 29) / 2;  // "RedBull OS gives your code wings!" is 29 chars
    terminal_writestring("RedBull OS gives your code wings!");
    
    // Wing animation position
    size_t wing_row = 12;
    size_t wing_left_col = (VGA_WIDTH / 2) - 15;
    size_t wing_right_col = (VGA_WIDTH / 2) + 5;
    
    // Run the wing animation several times
    terminal_setcolor(logo_color);
    for (int cycles = 0; cycles < 3; cycles++) {
        for (int frame = 0; frame < NUM_FRAMES; frame++) {
            // Draw left wing
            terminal_row = wing_row;
            terminal_column = wing_left_col;
            terminal_writestring(wing_frames[frame]);
            
            // Draw right wing (mirror of left wing)
            terminal_row = wing_row;
            terminal_column = wing_right_col;
            terminal_writestring(wing_frames[frame]);
            
            // Delay between frames - adjust as needed
            delay(5000000);
        }
    }
    
    // Reset position for normal output
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    terminal_row = 20;
    terminal_column = 0;
}

// Kernel entry point
int main(uint32_t magic, void* mb_info_addr) {
    // Display RedBull OS logo and loading animation
    display_redbull_logo();
    
    // Initialize GDT
    gdt_init();
    
    // Print success message
    terminal_writestring("GDT initialized. RedBull OS is now flying high!\n");
    
    // Loop forever
    while (1) {}
    
    return 0;
}