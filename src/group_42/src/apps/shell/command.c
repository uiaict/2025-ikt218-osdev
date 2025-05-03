#include "shell/command.h"
#include "kernel/print.h"
#include "shell/shell.h"
#include "command.h"

static int command_count = 0;
static command_t registry[MAX_COMMANDS];

void init_commands() {
  reg_command("help", help);
  reg_command("clear", clear_shell);
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

bool strcmp(const char *str1, const char *str2) {
  int i = 0;
  while (str1[i] != '\0' && str2[i] != '\0') {
    if (str1[i] != str2[i]) {
      return false;
    }
    i++;
  }
  return str1[i] == str2[i];
}

void help() {
  print("Available commands:\n");
  for (int i = 0; i < command_count; i++) {
    printf("- %s\n", registry[i].name);
  }
}