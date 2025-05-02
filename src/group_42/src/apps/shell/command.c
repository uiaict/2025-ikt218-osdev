#include "shell/command.h"
#include "kernel/print.h"
#include "shell/shell.h"

void run_command(Command command, char *input) {
  switch (command) {
  case HELP:
    print("Available commands: clear, song\n");
    break;
  case CLEAR:
    clear_shell();
    break;
  case SONG:
    print("Not implemented");
    break;
  default:
    printf("Invalid command '%s'\n", input);
    break;
  }
}

Command string_to_command(const char *string) {
  if (strcmp(string, "help") == 1) {
    return HELP;
  } else if (strcmp(string, "clear") == 1) {
    return CLEAR;
  } else if (strcmp(string, "song") == 1) {
    return SONG;
  } else {
    return -1;
  }
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