#include "libc/string.h"
#include "libc/stdbool.h"
#include "libc/stddef.h"

size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

// memmove: copy 'n' bytes, safe for overlapping regions
void *memmove(void *dest, const void *src, size_t n)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    if (d < s)
    {
        for (size_t i = 0; i < n; i++)
            d[i] = s[i];
    }
    else if (d > s)
    {
        for (size_t i = n; i-- > 0;)
            d[i] = s[i];
    }
    return dest;
}

// strtok: simple reentrant-like tokenizer using static state
char *strtok(char *str, const char *delim)
{
    static char *next;
    char *start;
    if (str)
    {
        next = str;
    }
    else if (!next)
    {
        return NULL;
    }

    // Skip any leading delimiters
    while (*next)
    {
        bool is_delim = false;
        for (const char *d = delim; *d; d++)
        {
            if (*next == *d)
            {
                is_delim = true;
                break;
            }
        }
        if (!is_delim)
            break;
        next++;
    }
    if (!*next)
    {
        next = NULL;
        return NULL;
    }

    // 'start' of the token
    start = next;

    // Find the end of it
    while (*next)
    {
        bool is_delim = false;
        for (const char *d = delim; *d; d++)
        {
            if (*next == *d)
            {
                is_delim = true;
                break;
            }
        }
        if (is_delim)
        {
            *next++ = '\0';
            return start;
        }
        next++;
    }

    // Last token (no trailing delimiter)
    next = NULL;
    return start;
}