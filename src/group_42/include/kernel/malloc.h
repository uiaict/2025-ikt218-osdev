#pragma once

/*
 This is a pretty terrible malloc, but it's a start.
*/

/**
 * @brief malloc.
 * @return pointer to memory address. Returns 0 if no memory is available.
 * @param bytes number of bytes to allocate.
 */
void *malloc(int bytes);

/**
 * @brief free.
 */
void free(void *pointer);