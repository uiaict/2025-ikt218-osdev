#include "shell/command.h"
#include "kernel/memory.h"
#include "kernel/pit.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "shell/shell.h"
#include "song_player/song_player.h"
#include "song_player/song_commands.h"

static int command_count = 0;
static command_t registry[MAX_COMMANDS];

void init_commands() {
  reg_command("help", list_commands);
  reg_command("clear", clear_shell);
  reg_command("memory", print_memory_layout);
  reg_command("sound", test_sound);
  reg_command("bf", play_bf1942);
  reg_command("starwars", play_starwars);
  reg_command("music1", play_music_1);
  reg_command("music2", play_music_2);
  reg_command("music3", play_music_3);
  reg_command("music4", play_music_4);
  reg_command("music5", play_music_5);
  reg_command("music6", play_music_6);
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