#ifndef SHELL_H
#define SHELL_H

#include "libc/stdbool.h"
#include "shell/command.h"

/**
 * @brief Flag to indicate if the shell is active.
 */
extern bool shell_active;

/**
 * @brief Initialize the shell.
 */
void shell_init();

/**
 * @brief Clear the shell screen.
 */
void clear_shell();

/**
 * @brief Handle input to the shell.
 * @param character The character input to the shell.
 */
void shell_input(char character);


#endif // SHELL_H