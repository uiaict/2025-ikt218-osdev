// Source file for memutils.c based on Per-Arne Andersen's implementation found at https://perara.notion.site/IKT218-Advanced-Operating-Systems-2024-bfa639380abd46389b5d69dcffda597a
#include "memory.h"
#include <libc/stddef.h>
#include <libc/stdint.h>
#include <libc/limits.h>
#include <libc/stdio.h>
#include <libc/string.h>

// Function to copy memory from source to destination
void* memcpy(void* dest, const void* src, size_t count ) {
    char* dst8 = (char*)dest;                          
    char* src8 = (char*)src;                           

    if (count & 1) {                                   
        dst8[0] = src8[0];                             
        dst8 += 1;                                     
        src8 += 1;                                     
    }

    count /= 2;                                        
    while (count--) {                                   
        dst8[0] = src8[0];                             
        dst8[1] = src8[1];                              

        dst8 += 2;                                      
        src8 += 2;                                     
    }

    return (void*)dest;                              
}


void* memset16 (void *ptr, uint16_t value, size_t num) {
    uint16_t* p = ptr;                               
    while(num--)
        *p++ = value;                                
    return ptr;                         
}

