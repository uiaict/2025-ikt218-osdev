#ifndef COMMAND_H
#define COMMAND_H

#include "libc/stdbool.h"

#define MAX_COMMANDS 100

/**
 * @brief Function pointer type for command functions.
 * Commands should take no arguments and return void.
 */
typedef void (*command_func_t)(void);

/**
 * @brief Structure to represent a command with its name and function.
 */
typedef struct {
  const char *name;
  command_func_t func;
} command_t;

/**
 * @brief Initialize the command registry.
 */
void init_commands();

/**
 * @brief Register a command to registry
 */
void reg_command(const char *name, command_func_t func);

/**
 * @brief Run a command.
 * @param input The input string associated with the command.
 */
void run_command(const char *input);

/**
 * @brief Display a list of available commands.
 */
void list_commands();

#endif