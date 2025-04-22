#include "libc/memory.h"   // Fix include path
#include "libc/common.h"    // For u32int definition

// Commenting out duplicate memcpy function - already defined in common.c
/*
void* memcpy(void* dest, const void* src, size_t count )
{
    char* dst8 = (char*)dest;  // Cast the destination pointer to char*
    char* src8 = (char*)src;   // Cast the source pointer to char*

    if (count & 1) {           // If the count is odd
        dst8[0] = src8[0];     // Copy one byte from source to destination
        dst8 += 1;             // Increment destination pointer by one byte
        src8 += 1;             // Increment source pointer by one byte
    }

    count /= 2;                // Divide the count by two to copy 2 bytes at a time
    while (count--) {          // Loop through each pair of 2 bytes
        dst8[0] = src8[0];     // Copy the first byte of the pair from source to destination
        dst8[1] = src8[1];     // Copy the second byte of the pair from source to destination

        dst8 += 2;             // Increment destination pointer by 2 bytes
        src8 += 2;             // Increment source pointer by 2 bytes
    }

    return (void*)dest;        // Return the destination pointer
}
*/

// Function to set a block of memory with a 16-bit value
void* memset16 (void *ptr, uint16_t value, size_t num)
{
    uint16_t* p = ptr;        // Cast the pointer to uint16_t*
    while(num--)
        *p++ = value;         // Set each 2-byte element to the given value
    return ptr;               // Return the pointer to the block of memory
}

// Commenting out duplicate memset function - already defined in idt/util.c
/*
void* memset(void *dest, unsigned char val, u32int n)
{
    unsigned char* p = dest;     // Cast the pointer to unsigned char*
    while(n--)
        *p++ = val;   // Set each byte to the given value
    return dest;      // Return the pointer to the block of memory
}
*/
