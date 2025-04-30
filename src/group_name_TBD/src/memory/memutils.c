#include "memory/memutils.h"

void* memcpy(void* dest, const void* src, size_t count){

    char* dst8 = (char*)dest;       // Cast the destination pointer to char*
    const char* src8 = (char*)src;  // Cast the source pointer to char*

    for (size_t i = 0; i < count; i++){
        dst8[i] = src8[i];
    }
    
    return (void*)dest;        // Return the destination pointer
}



void* memset16(void *ptr, uint16_t value, size_t count)
{
    uint16_t *p = ptr;        // Cast the pointer to uint16_t*

    for (size_t i = 0; i < count; i++){
        p[i] = value;
    }

    return ptr;               // Return the pointer to the block of memory
}




void* memset(void * ptr, uint8_t value, size_t count){

    uint8_t *p = ptr;     // Cast the pointer to unsigned char*

    for (size_t i = 0; i < count; i++){
        p[i] = value;
    }

    return ptr;         // Return the pointer to the block of memory
}