#pragma once
#include "libc/stdint.h"
#define HASH_TABLE_SIZE 100
typedef struct KeyValuePair
{
    char *key;
    char *value;
    struct KeyValuePair *next;
} KeyValuePair;

typedef struct Dictionary
{
    KeyValuePair *table[HASH_TABLE_SIZE];
} Dictionary;
int strcmp(const char *str1, const char *str2);
uint32_t hash(const char *key);
KeyValuePair *create_pair(const char *key, const char *value);
void insert(Dictionary *dict, const char *key, const char *value);
char *get(Dictionary *dict, char *key);
