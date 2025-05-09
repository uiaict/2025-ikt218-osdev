#include "memory.h"

// Copies 'count' bytes from 'src' to 'dest'
void* memcpy(void* dest, const void* src, size_t count) {
    char* dst8 = (char*)dest; // Destination byte pointer
    char* src8 = (char*)src;  // Source byte pointer

    // If 'count' is odd, copy first byte to make the remaining count even for pair processing
    if (count & 1) {
        *dst8++ = *src8++;
    
    }

    count /= 2;   // Calculate number of 2-byte pairs
    while (count--) {
        *dst8++ = *src8++;
        *dst8++ = *src8++;
    }

    return dest;
}

// Fills 'num' 16-bit words at 'ptr' with 'value'
void* memset16 (void *ptr, uint16_t value, size_t num)
{
    uint16_t* p = (uint16_t*)ptr;
    while(num--) {
        *p++ = value;
    }
    return ptr;
}

// Fills 'num' bytes at 'ptr' with 'value'
void* memset (void * ptr, int value, size_t num)
{
    unsigned char* p = (unsigned char*)ptr;
    unsigned char byte_value = (unsigned char)value;

    while(num--) {
        *p++ = byte_value;
    }
    return ptr;
}