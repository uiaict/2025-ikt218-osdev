#include "shell/shell.h"
#include "libc/stdio.h"
#include "kernel/system.h"
#include "libc/stdint.h"
#include "kernel/keyboard.h"

char input_buffer[SCREEN_WIDTH];

void shell_init() {
  clear_shell();

  cursor_enable(0, 0);
  update_cursor(0, 1);

  init_commands();
  input_set_keyboard_subscriber(shell_input);
}

void clear_shell() {
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    clear_line(i);
  }
  cursorPositionX_ = 0;
  cursorPositionY_ = 0;
}

void shell_input(char character) {
  if (character != '\n' && character != 0x08) {
    volatile char *video =
        cursorPosToAddress(cursorPositionX_, cursorPositionY_);
    *video = character;
    video++;
    *video = VIDEO_WHITE;
    input_buffer[cursorPositionX_] = character;

    incrementCursorPosition();
    update_cursor(cursorPositionX_, cursorPositionY_ + 1);
  } else if (character == 0x08) {
    update_cursor(cursorPositionX_ - 1, cursorPositionY_ + 1);
    if (cursorPositionX_ != 0) {
      cursorPositionX_--;
    }

    volatile char *video =
        cursorPosToAddress(cursorPositionX_, cursorPositionY_);
    *video = 0;
    video++;
    *video = VIDEO_WHITE;
    input_buffer[cursorPositionX_ - 1] = 0;
  } else if (character == '\n') {
    cursorPositionY_++;
    cursorPositionX_ = 0;
    run_command(input_buffer);
    for (int i = 0; i < SCREEN_WIDTH; i++) {
      input_buffer[i] = 0;
    }
    update_cursor(cursorPositionX_, cursorPositionY_ + 1);
  }
}
