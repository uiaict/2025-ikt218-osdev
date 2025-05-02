#ifndef COMMAND_H
#define COMMAND_H

#include "libc/stdbool.h"

/**
 * @brief Enum for shell commands.
 */
typedef enum { HELP, CLEAR, SONG } Command;

/**
 * @brief Compare two strings.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return True if the strings are equal, false otherwise.
 */
bool strcmp(const char *str1, const char *str2);

/**
 * @brief Convert a string to a command.
 * @param string The string to convert.
 * @return The corresponding command enum.
 */
Command string_to_command(const char *string);

/**
 * @brief Run a command.
 * @param command The command to run.
 * @param input The input string associated with the command.
 */
void run_command(Command command, char *input);

#endif