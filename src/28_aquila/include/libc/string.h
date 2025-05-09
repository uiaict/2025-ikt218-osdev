#pragma once

#ifndef STRING_H
#define STRING_H

int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, int n);
int strlen(const char* str);
int strcpy(char* dest, const char* src);
char *strncpy(char *dest, const char *src, int n);
int startsWith(const char *str, const char *prefix);


#endif
