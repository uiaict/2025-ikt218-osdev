#pragma once

#include "libc/stdbool.h"
#include "libc/stdint.h"

/**
 *
 * @brief Calculate the length of a string.
 * @param str The input string.
 * @return The length of the string.
 */
size_t strlen(const char *str);

/**
 *
 * @brief Compare two strings.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return True if the strings are equal, false otherwise.
 */
bool strcmp(const char *str1, const char *str2);