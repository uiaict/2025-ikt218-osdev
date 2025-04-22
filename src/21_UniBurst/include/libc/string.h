#pragma once
#include "libc/stdint.h"

size_t strlen(char *str); 
char *strchr(char *str, int c); 
void strrev(char str[], int length); 
void *memset(void *str, int c, size_t n); 

