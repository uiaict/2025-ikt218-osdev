#pragma once

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/limits.h"
#include "libc/stdio.h"
#include "string.h"

#define EOF (-1)

// Stops the system with an error message
void panic(const char* reason);

// Converts 32-bit unsigned int to hex string
char* hex32_to_str(char buffer[], unsigned int val);

// Converts 32-bit signed int to decimal string
char* int32_to_str(char buffer[], int val);
