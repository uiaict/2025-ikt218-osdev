#ifndef UTILS_H
#define UTILS_H


#include "libc/stdint.h"


void memset(void *ptr, char value, uint32_t count);
void outPortB(uint16_t port, uint8_t value);
uint8_t inPortB(uint16_t port);
void get_input(char* buffer, int size);
int stoi(const char *str);


#endif