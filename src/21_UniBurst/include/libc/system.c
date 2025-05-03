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
    int i;
    
    // Start at the end of the buffer and work backwards
    buffer[8] = '\0';
    for(i = 7; i >= 0; i--) {
        buffer[i] = hex_chars[val & 0xF];
        val >>= 4;
    }
    
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