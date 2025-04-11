 #include "monitor.h"
 #include "system.h"
 
 /*==============================================================================
  * VGA Constants and Globals
  *============================================================================*/

 enum vga_color 
 {
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
 
 static const size_t VGA_WIDTH  = 80;

 static const size_t VGA_HEIGHT = 25;
 
 uint16_t *video_memory = (uint16_t *)0xB8000;
 size_t terminal_row = 0;
 size_t terminal_column = 0;

 uint8_t terminal_color = 0;
 uint16_t* terminal_buffer = NULL;
 
 /*==============================================================================
  * Helper Functions
  *============================================================================*/
 
 /* Inline function: Combines foreground and background colours into a single attribute. */
 static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
 {
     return fg | (bg << 4);
 }
 
 /* Inline function: Creates a VGA entry from a character and its colour attribute. */
 static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
 {
     return uc | (color << 8);
 }
 
 /*
  * scroll()
  *
  * Scrolls the text on the screen up by one line when the terminal_row exceeds the height.
  */
 static void scroll(void) 
 {
     // Create a blank space entry with default colours (black background, white foreground).
     uint8_t attributeByte = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
     uint16_t blank = 0x20 | (attributeByte << 8);
 
     if (terminal_row >= VGA_HEIGHT) {
         // Move each line one row upward.
         for (size_t i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) 
         {
             terminal_buffer[i] = terminal_buffer[i + VGA_WIDTH];
         }
         // Clear the last line.
         for (size_t i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) 
         {
             terminal_buffer[i] = blank;
         }
         terminal_row = VGA_HEIGHT - 1;
     }
 }
 
 /*
  * move_cursor()
  *
  * Updates the hardware cursor position based on terminal_row and terminal_column.
  */
 static void move_cursor(void) 
 {
     uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;
     outb(0x3D4, 0x0F);
     outb(0x3D5, (uint8_t)(pos & 0xFF));
     outb(0x3D4, 0x0E);
     outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
 }
 

 /*==============================================================================
  * Monitor Initialization and Basic Functions
  *============================================================================*/
 
 /*
  * monitor_initialize()
  *
  * Initializes the terminal: resets the cursor, sets the default colour,
  * and clears the screen.
  */

 void monitor_initialize(void) 
 {
     terminal_row = 0;
     terminal_column = 0;
     terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
     terminal_buffer = video_memory;
 
     for (size_t y = 0; y < VGA_HEIGHT; y++) {
         for (size_t x = 0; x < VGA_WIDTH; x++) {
             size_t index = y * VGA_WIDTH + x;
             terminal_buffer[index] = vga_entry(' ', terminal_color);
         }
     }
 }
 
 /*
  * monitor_setcolor()
  *
  * Sets the terminal text colour.
  */
 void monitor_setcolor(uint8_t color) 
 {
     terminal_color = color;
 }
 
 /*
  * monitor_putentryat()
  *
  * Writes character 'c' with given colour at position (x, y).
  */
 void monitor_putentryat(char c, uint8_t color, size_t x, size_t y) 
 {
     size_t index = y * VGA_WIDTH + x;
     terminal_buffer[index] = vga_entry(c, color);
 }
 
 /*
  * _monitor_put()
  *
  * Internal function to handle printing a character, including special characters
  * such as newline. It updates the terminal state but does not move the cursor.
  */
 void _monitor_put(char c) 
 {
     switch (c) {
         case '\n':
             terminal_column = 0;
             terminal_row++;
             scroll();
             return;
         default:
             break;
     }
 
     monitor_putentryat(c, terminal_color, terminal_column, terminal_row);
     if (++terminal_column >= VGA_WIDTH) 
     {
         terminal_column = 0;
         terminal_row++;
         if (terminal_row >= VGA_HEIGHT)
             terminal_row = VGA_HEIGHT - 1;
     }
 }
 
 /*
  * monitor_put()
  *
  * Public function to print a character and then update the display and hardware cursor.
  */
 void monitor_put(char c) 
 {
     _monitor_put(c);
     scroll();
     move_cursor();
 }
 
 /*
  * monitor_write()
  *
  * Writes a block of characters (of specified size) to the screen.
  */
 void monitor_write(const char* data, size_t size) 
 {
     for (size_t i = 0; i < size; i++)
     {
         _monitor_put(data[i]);
     }
     scroll();
     move_cursor();
 }
 
 /*
  * monitor_writestring()
  *
  * Writes a null-terminated string to the screen.
  */
 void monitor_writestring(const char* data) 
 {
     monitor_write(data, strlen(data));
 }
 
 /*
  * monitor_clear()
  *
  * Clears the screen by filling it with blank spaces and resets the cursor.
  */
 void monitor_clear(void) 
 {
     uint8_t attributeByte = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
     uint16_t blank = 0x20 | (attributeByte << 8);
 
     for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) 
     {
         terminal_buffer[i] = blank;
     }
     terminal_row = 0;
     terminal_column = 0;
     move_cursor();
 }
 
 /*==============================================================================
  * Number Printing Functions
  *============================================================================*/
 
 /*
  * monitor_write_hex()
  *
  * Writes a 32-bit unsigned number in hexadecimal format to the screen.
  */
 void monitor_write_hex(uint32_t n) 
 {
     monitor_writestring("0x");
     int32_t tmp;
     char noZeroes = 1;
 
     for (int i = 28; i > 0; i -= 4) 
     {
         tmp = (n >> i) & 0xF;
         if (tmp == 0 && noZeroes)
             continue;
         noZeroes = 0;
         if (tmp >= 0xA)
             monitor_put(tmp - 0xA + 'a');
         else
             monitor_put(tmp + '0');
     }
 
     tmp = n & 0xF;
     if (tmp >= 0xA)
         monitor_put(tmp - 0xA + 'a');
     else
         monitor_put(tmp + '0');
 }
 
 /*
  * monitor_write_dec()
  *
  * Writes a 32-bit unsigned number in decimal format to the screen.
  */
 void monitor_write_dec(uint32_t n) 
 {
     if (n == 0) {
         monitor_put('0');
         return;
     }
 
     int32_t acc = n;
     char c[32];
     int i = 0;
     while (acc > 0) {
         c[i++] = '0' + (acc % 10);
         acc /= 10;
     }
     c[i] = '\0';
 
     // Reverse the string.
     char reversed[32];
     int j = 0;
     for (int k = i - 1; k >= 0; k--) 
     {
         reversed[j++] = c[k];
     }
     reversed[j] = '\0';
 
     monitor_writestring(reversed);
 }
 