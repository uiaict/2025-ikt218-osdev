#include "../memory/memory.h"
#include <stdint.h>
#include <stddef.h>

// Function to copy memory from source to destination
void* memcpy(void* dest, const void* src, size_t count)
{
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

// Function to set a block of memory with a 16-bit value
void* memset16(void *ptr, uint16_t value, size_t num)
{
    uint16_t* p = ptr;
    while(num--)
        *p++ = value;
    return ptr;
}

// Function to set a block of memory with a byte value
void* krnl_memset(void *ptr, int value, size_t num)
{
    unsigned char* p = ptr;
    while(num--)
        *p++ = (unsigned char)value;
    return ptr;
}