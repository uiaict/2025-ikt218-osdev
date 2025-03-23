#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/system.h"
#include "libc/stdarg.h"

int putchar(int ic)
{
    char c = (char)ic;
    monitor_put(c);
    return ic;
}

bool print(const char *data, size_t length)
{
    const unsigned char *bytes = (const unsigned char *)data;
    for (size_t i = 0; i < length; i++)
        if (putchar(bytes[i]) == EOF)
            return false;
    return true;
}

int printf(const char *__restrict__ format, ...)
{
    char *video_memory = (char *)0xb8000;
    size_t len = strlen(format);

    for (size_t i = 0; i < len; i++)
    {
        video_memory[i * 2] = format[i];
        video_memory[i * 2 + 1] = 0x07;
    }
    return 0;
}