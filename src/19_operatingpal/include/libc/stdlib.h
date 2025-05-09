#pragma once

#include "libc/stdbool.h"

// Converts signed int to string (base 10, 16, etc.)
char *itoa(int num, char* buffer, int base);

// Converts unsigned int to string
char *utoa(unsigned int num, char* buffer, int base);

// Converts string to int
int atoi(char *str);

// Converts float to string with decimals
void ftoa(float n, char* res, int afterpoint);
