#pragma once

#include "libc/stdbool.h"

char *itoa(int num, char* buffer, int base); 
char *utoa(unsigned int num, char* buffer, int base); 
int atoi(char *str); 
void ftoa(float n, char* res, int afterpoint); 


