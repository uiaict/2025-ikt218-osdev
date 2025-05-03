#include "shell/command.h"
#include "libc/stdio.h"
#include "shell/shell.h"
#include "kernel/memory.h"
#include "libc/string.h"

static int command_count = 0;
static command_t registry[MAX_COMMANDS];

void init_commands() {
  reg_command("help", list_commands);
  reg_command("clear", clear_shell);
  reg_command("memory", print_memory_layout);
}

void reg_command(const char *name, command_func_t func) {

  if (command_count >= MAX_COMMANDS) {
    print("Command registry full\n");
    return;
  }
  registry[command_count++] = (command_t){name, func};
}

void run_command(const char *input) {
  for (int i = 0; i < command_count; i++) {
    if (strcmp(input, registry[i].name)) {
      registry[i].func();
      return;
    }
  }
  printf("Command '%s' not found, type 'help'\n", input);
}

void list_commands() {
  print("Available commands:\n");
  for (int i = 0; i < command_count; i++) {
    printf("- %s\n", registry[i].name);
  }
}