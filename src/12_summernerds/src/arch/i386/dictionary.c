#include "i386/dictionary.h"
#include "/"

int strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2 && *str1 == *str2)
    {
        str1++;
        str2++;
    }

    return *(unsigned char *)str1 - *(unsigned char *)str2;
}
// Hash function
uint32_t hash(const char *key)
{
    uint32_t hash = 0;
    while (*key)
    {
        hash = (hash << 5) + *key++;
    }
    return hash % HASH_TABLE_SIZE;
}

// Create a new key-value pair
KeyValuePair *create_pair(const char *key, const char *value)
{
    KeyValuePair *pair = (KeyValuePair *)custom_malloc(sizeof(KeyValuePair));
    if (!pair)
    {
        return NULL;
    }
    pair->key = key;
    pair->value = value;
    pair->next = NULL;
    return pair;
}

// Insert a key-value pair into the dictionary
void insert(Dictionary *dict, const char *key, const char *value)
{
    uint32_t index = hash(key);
    KeyValuePair *pair = create_pair(key, value);
    if (!pair)
    {
        return; // Error: unable to create pair
    }
    pair->next = dict->table[index];
    dict->table[index] = pair;
}

// Lookup the value associated with a key
char *get(Dictionary *dict, char *key)
{
    uint32_t index = hash(key);
    KeyValuePair *pair = dict->table[index];
    while (pair)
    {
        if (strcmp(pair->key, key) == 0)
        {
            return pair->value;
        }
        pair = pair->next;
    }
    return NULL; // Key not found
}