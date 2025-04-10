#include "state/state.h"
#include "state/shell_command.h"
#include "interrupts/keyboard/keyboard.h"
#include "main_menu/main_menu.h"
#include "screens/screens.h"

#include "terminal/print.h"
#include "terminal/cursor.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"

static volatile SystemState current_state = NOT_USED;
static volatile SystemState previous_state = NOT_USED;

static bool same_state_check(void) {
    return current_state == previous_state;
}

SystemState get_current_state(void) {
    return current_state;
}

void change_state(SystemState new_state) {
    if (new_state != current_state) {
        current_state = new_state;
    }
}

void update_state(void) {
    switch (current_state)
    {
    case NOT_USED: {
        change_state(START_SCREEN);
        break;
    }

    case START_SCREEN: {
        if (same_state_check()) {
            if (keyboard_has_char()) {
                char c = keyboard_get_char();
                if (c == '\r') {
                    change_state(SHELL);
                }
            }
            break;
        }
        previous_state = current_state;
        start_screen_reveal();
        break;
    }
    case SHELL: {
        if (same_state_check()) {
            if (keyboard_has_char()) {
                char c = keyboard_get_char();
                if (c != '\x1B') {
                    printf("%c", c);
                    if (c == '\r') {
                        ShellCommand_t cmd = get_shell_command();
                        switch (cmd) {
                            case LOAD_STATIC_SCREEN:
                                change_state(STATIC_SCREEN);
                                break;
                            case LOAD_MUSIC_PLAYER:
                                change_state(MUSIC_PLAYER);
                                break;
                            default:
                                break;
                        }
                    }
                } else {
                    if (keyboard_has_char()) {
                        char d = keyboard_get_char();
                        if (d == 'D') {
                            move_cursor_left();
                        } else if (d == 'C') {
                            move_cursor_right();
                        }
                    }
                }
            }
            break;
        }

        previous_state = current_state;
        clearTerminal();
        break;
    }
    case STATIC_SCREEN: {
        if (same_state_check()) {
            if (keyboard_has_char()) {
                char c = keyboard_get_char();
                if (c == '\x1B' && keyboard_has_char() == false)  {
                    change_state(SHELL);
                }
            }
            break;
        }
        previous_state = current_state;
        char* command = get_shell_command_string();
        if (command != NULL) {
            if (strcmp(command, "info") == 0) {
                print_about_screen();
            } else if (strcmp(command, "command-ls") == 0) {
                print_command_help();
            }
        }
        break;
    }
    
    default:
        __asm__ volatile ("hlt");
        break;
    }
}