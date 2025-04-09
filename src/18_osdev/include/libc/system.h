#pragma once

#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "limits.h"
#include "stdio.h"



#include "string.h"

#define EOF (-1)


// Custom Functions
void panic(const char* reason);
char* hex32_to_str(char buffer[], unsigned int val);
char* int32_to_str(char buffer[], int val);

