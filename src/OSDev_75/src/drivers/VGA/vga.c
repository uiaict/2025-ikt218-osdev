
#include <stddef.h>
#include "vga.h"
#include "util.h" // for outPortB, inPortB, etc.
#include <stdarg.h>

// Simple printf implementation for debugging
int printf(const char* format, ...) {
    // Buffer for formatted output
    char buffer[256];
    int i = 0;
    
    va_list args;
    va_start(args, format);
    
    // Very simple formatting - only handles %d, %x, %s
    while (format[i]) {
        if (format[i] == '%' && format[i+1]) {
            i++;
            switch(format[i]) {
                case 'd': {
                    int val = va_arg(args, int);
                    char num_buffer[12];
                    int j = 0;
                    int is_negative = 0;
                    
                    if (val < 0) {
                        is_negative = 1;
                        val = -val;
                    }
                    
                    // Convert to string
                    do {
                        num_buffer[j++] = '0' + (val % 10);
                        val /= 10;
                    } while (val);
                    
                    // Add negative sign if needed
                    if (is_negative) {
                        print("-");
                    }
                    
                    // Print in reverse (correct order)
                    while (j > 0) {
                        char c[2] = {num_buffer[--j], '\0'};
                        print(c);
                    }
                    break;
                }
                case 's': {
                    char* str = va_arg(args, char*);
                    print(str);
                    break;
                }
                case 'x': {
                    int val = va_arg(args, int);
                    char num_buffer[12];
                    int j = 0;
                    
                    // Convert to hex string
                    do {
                        int digit = val & 0xF;
                        num_buffer[j++] = digit < 10 ? '0' + digit : 'a' + (digit - 10);
                        val >>= 4;
                    } while (val);
                    
                    print("0x");
                    
                    // Print in reverse (correct order)
                    while (j > 0) {
                        char c[2] = {num_buffer[--j], '\0'};
                        print(c);
                    }
                    break;
                }
                default:
                    // Handle % character
                    char c[2] = {format[i], '\0'};
                    print(c);
                    break;
            }
        } else {
            // Regular character
            char c[2] = {format[i], '\0'};
            print(c);
        }
        i++;
    }
    
    va_end(args);
    return i;
}

// Simple panic function
void panic(const char* message) {
    // Set color to bright red on black for panic messages
    setColor(COLOR8_LIGHT_RED, COLOR8_BLACK);
    print("\n*** KERNEL PANIC ***\n");
    print(message);
    print("\nSystem halted\n");
    
    // Infinite loop to halt execution
    while(1) {
        __asm__ __volatile__("hlt");
    }
}
// -----------------------------
// Hardware cursor helper funcs
// -----------------------------
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    outPortB(0x3D4, 0x0A);
    outPortB(0x3D5, (inPortB(0x3D5) & 0xC0) | cursor_start);

    outPortB(0x3D4, 0x0B);
    outPortB(0x3D5, (inPortB(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(uint16_t x, uint16_t y)
{
    uint16_t pos = y * VGA_WIDTH  + x;

    outPortB(0x3D4, 0x0F);
    outPortB(0x3D5, (uint8_t)(pos & 0xFF));
    outPortB(0x3D4, 0x0E);
    outPortB(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// -----------------------------
// VGA text-mode driver
// -----------------------------
static uint16_t* const VGA_MEMORY = (uint16_t* const)0xB8000;

static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;

static const uint8_t DEFAULT_ATTRIBUTE = ((0 & 0x0F) << 4) | (15 & 0x0F);
static uint8_t currentAttribute = 0;

static inline uint16_t makeVgaCell(char c, uint8_t attr)
{
    return (uint16_t)(c & 0xFF) | ((uint16_t)attr << 8);
}

void setColor(uint8_t fg, uint8_t bg)
{
    currentAttribute = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

void Reset(void)
{
    cursor_x = 0;
    cursor_y = 0;
    currentAttribute = DEFAULT_ATTRIBUTE;

    for (uint16_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (uint16_t x = 0; x < VGA_WIDTH ; x++)
        {
            VGA_MEMORY[y * VGA_WIDTH  + x] = makeVgaCell(' ', DEFAULT_ATTRIBUTE);
        }
    }

    // Enable hardware cursor (underline style)
    enable_cursor(14, 15);
    // Update cursor to (0, 0)
    update_cursor(cursor_x, cursor_y);
}

void scrollUp(void)
{
    for (uint16_t y = 1; y < VGA_HEIGHT; y++)
    {
        for (uint16_t x = 0; x < VGA_WIDTH ; x++)
        {
            VGA_MEMORY[(y - 1) * VGA_WIDTH  + x] = VGA_MEMORY[y * VGA_WIDTH  + x];
        }
    }
    // Clear last line
    for (uint16_t x = 0; x < VGA_WIDTH ; x++)
    {
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH  + x] = makeVgaCell(' ', currentAttribute);
    }
}

void newLine(void)
{
    cursor_x = 0;
    if (cursor_y < VGA_HEIGHT - 1)
    {
        cursor_y++;
    }
    else
    {
        scrollUp();
    }
    // Move hardware cursor
    update_cursor(cursor_x, cursor_y);
}

void print(const char* text)
{
    while (*text)
    {
        char c = *text++;

        if (c == '\n')
        {
            newLine();
        }
        else if (c == '\r')
        {
            cursor_x = 0;
            update_cursor(cursor_x, cursor_y);
        }
        else if (c == '\b')
        {
            // Backspace
            if (cursor_x > 0)
            {
                cursor_x--;
                VGA_MEMORY[cursor_y * VGA_WIDTH  + cursor_x] = makeVgaCell(' ', currentAttribute);
            }
            else if (cursor_y > 0)
            {
                // Optional: handle backspace at start of line
                cursor_y--;
                cursor_x = VGA_WIDTH  - 1;
                VGA_MEMORY[cursor_y * VGA_WIDTH  + cursor_x] = makeVgaCell(' ', currentAttribute);
            }
            update_cursor(cursor_x, cursor_y);
        }
        else
        {
            // Normal character
            if (cursor_x >= VGA_WIDTH )
            {
                newLine();
            }
            VGA_MEMORY[cursor_y * VGA_WIDTH  + cursor_x] = makeVgaCell(c, currentAttribute);
            cursor_x++;
            update_cursor(cursor_x, cursor_y);
        }
    }
}

// Delay for ASCII animation
static void delaySpin(unsigned long count)
{
    for (volatile unsigned long i = 0; i < count; i++)
    {
        __asm__ __volatile__("nop");
    }
}
// ASCII frames (unchanged as requested)
static const char* FRAMES[4] =
{
    // Frame 1
    " /$$   /$$ /$$$$$$  /$$$$$$ \n"
    "| $$  | $$|_  $$_/ /$$__  $$\n"
    "| $$  | $$  | $$  | $$  \\ $$\n"
    "| $$  | $$  | $$  | $$$$$$$$\n"
    "| $$  | $$  | $$  | $$__  $$\n"
    "| $$  | $$  | $$  | $$  | $$\n"
    "|  $$$$$$/ /$$$$$$| $$  | $$\n"
    " \\______/ |______/|__/  |__/\n"
    "\n",

    // Frame 2
    "$$\\   $$\\ $$$$$$\\  $$$$$$\\  \n"
    "$$ |  $$ |\\_$$  _|$$  __$$\\ \n"
    "$$ |  $$ |  $$ |  $$ /  $$ |\n"
    "$$ |  $$ |  $$ |  $$$$$$$$ |\n"
    "$$ |  $$ |  $$ |  $$  __$$ |\n"
    "$$ |  $$ |  $$ |  $$ |  $$ |\n"
    "\\$$$$$$  |$$$$$$\\ $$ |  $$ |\n"
    " \\______/ \\______/\\__|  \\__|\n"
    "\n",

    // Frame 3
    " __    __  ______   ______  \n"
    "|  \\  |  \\|      \\ /      \\ \n"
    "| $$  | $$ \\$$$$$$|  $$$$$$\\\n"
    "| $$  | $$  | $$  | $$__| $$\n"
    "| $$  | $$  | $$  | $$    $$\n"
    "| $$  | $$  | $$  | $$$$$$$$\n"
    "| $$__/ $$ _| $$_ | $$  | $$\n"
    " \\$$    $$|   $$ \\| $$  | $$\n"
    "  \\$$$$$$  \\$$$$$$ \\$$   \\$$\n"
    "\n",

    // Frame 4
    " /$$   /$$ /$$$$$$  /$$$$$$ \n"
    "| $$  | $$|_  $$_/ /$$__  $$\n"
    "| $$  | $$  | $$  | $$  \\ $$\n"
    "| $$  | $$  | $$  | $$$$$$$$\n"
    "| $$  | $$  | $$  | $$__  $$\n"
    "| $$  | $$  | $$  | $$  | $$\n"
    "|  $$$$$$/ /$$$$$$| $$  | $$\n"
    " \\______/ |______/|__/  |__/\n"
    "\n"
};

#define NUM_FRAMES (sizeof(FRAMES) / sizeof(FRAMES[0]))

//----------------------------------------------
// show_animation: show each frame with dark red background and white text
void show_animation(void)
{
    // Use size_t to match NUM_FRAMES (avoids sign-compare warning)
    for (size_t i = 0; i < NUM_FRAMES; i++)
    {
        // Set color to white text on dark red background for all frames
        setColor(COLOR8_WHITE, COLOR8_DARK_GREY);

        // Clear
        Reset();

        // Print this ASCII chunk
        print(FRAMES[i]);

        // Delay
        delaySpin(200000000);
    }

    // Revert to default (white text on black background) for "hello world"
    setColor(COLOR8_WHITE, COLOR8_BLACK);
    Reset();
}
//------------------------------------------------

void putCharAt(uint16_t x, uint16_t y, char c, uint8_t fg, uint8_t bg) {
    // Check if x and y are within bounds
    if (x >= VGA_WIDTH  || y >= VGA_HEIGHT) {
        return;
    }
    
    // Calculate attribute byte
    uint8_t attr = ((bg & 0x0F) << 4) | (fg & 0x0F);
    
    // Put character at the specified position
    VGA_MEMORY[y * VGA_WIDTH  + x] = makeVgaCell(c, attr);
}

void setCursorPosition(uint16_t x, uint16_t y) {
    // Check if x and y are within bounds
    if (x >= VGA_WIDTH  || y >= VGA_HEIGHT) {
        return;
    }
    
    // Set internal cursor position
    cursor_x = x;
    cursor_y = y;
    
    // Update hardware cursor
    update_cursor(x, y);
}

void getCursorPosition(uint16_t* x, uint16_t* y) {
    if (x) {  // Instead of NULL check, just check if pointer is non-zero
        *x = cursor_x;
    }
    
    if (y) {  // Instead of NULL check, just check if pointer is non-zero
        *y = cursor_y;
    }
}

uint16_t getScreenWidth() {
    return VGA_WIDTH ;
}

uint16_t getScreenHeight() {
    return VGA_HEIGHT;
}