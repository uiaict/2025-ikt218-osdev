#include "libc/string.h"
#include "libc/stdint.h"

size_t strlen(cosnt char* str){
    size_t len = 0;
    while (str[len])
    {
        len++;
    }
    return len;    
}