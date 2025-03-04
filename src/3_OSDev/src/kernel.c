#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

#include <multiboot2.h>

#include <gdt.h>

#define VGA_MEMORY 0xB8000
#define screen_width 80
int x = 0;
int y = 0;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Sources: http://www.brokenthorn.com/Resources/OSDev10.html
//          https://wiki.osdev.org/Printing_To_Screen
void terminal_write( int colour, const char *string ) {
    int memory_location;                                                // Declares an integer var   
    memory_location = VGA_MEMORY + (x + (y * screen_width) * 2);        // Calculates the memory location (based of source) 
                                                                            // Multiplication by 2 is because each character is 2 bytes
    volatile char *video = (volatile char*)memory_location;             // Declares a volatile char pointer and assigns the memory location to it
    while (*string != 0) {                                              
        *video++ = *string++;                                           // Writes the string to the video memory
        *video++ = colour;                                              // Writes the colour to the video memory              
    }   
    y++;                                                                // Increments the y value           
}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    // Initialize GDT
    init_gdt();
    
    // Print "Hello World!" to screen
    terminal_write(0x0F, "Hello World!");
    return 0;

}