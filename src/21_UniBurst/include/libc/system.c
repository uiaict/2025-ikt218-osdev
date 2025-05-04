#include "system.h"
#include "string.h"
#include "stdint.h"
#include "stddef.h"

// Panic function - halts the system with an error message
//void panic(const char* reason) {
   // asm volatile("cli");
   // while(1) {
        //asm volatile("hlt");
    //}
//}

// Convert a 32-bit hex value to a string
char* hex32_to_str(char buffer[], unsigned int val) {
    const char* hex_chars = "0123456789ABCDEF";
    
    // Add "0x" prefix
    buffer[0] = '0';
    buffer[1] = 'x';
    
    // Fill in hex digits
    for (int i = 0; i < 8; i++) {
        int digit = (val >> (28 - i * 4)) & 0xF;
        buffer[2 + i] = hex_chars[digit];
    }
    
    buffer[10] = '\0';
    return buffer;
}

// Convert a 32-bit integer to a string
char* int32_to_str(char buffer[], int val) {
    int i = 0;
    int is_negative = 0;
    
    // Handle 0 explicitly
    if (val == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return buffer;
    }
    
    // Handle negative numbers
    if (val < 0) {
        is_negative = 1;
        val = -val;
    }
    
    while (val != 0) {
        buffer[i++] = (val % 10) + '0';
        val = val / 10;
    }
    

    if (is_negative) {
        buffer[i++] = '-';
    }
    
    buffer[i] = '\0';
    strrev(buffer, i);
    
    return buffer;
}