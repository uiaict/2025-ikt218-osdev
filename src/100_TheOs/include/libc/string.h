#ifndef STRING_H
#define STRING_H

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t strlen(const char* str);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);

#ifdef __cplusplus
}
#endif

#endif // STRING_H
