#include "state.h"
#include "interrupts/keyboard/keyboard.h"
#include "main_menu/main_menu.h"
#include "about_screen/about_screen.h"

#include "start_screen/start_screen.h"
#include "terminal/print.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"


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
                    change_state(MENU);
                }
            }
            break;
        }
        previous_state = current_state;
        start_screen_reveal();
        break;
    }

    case MENU: {
        if (same_state_check()) {
            if (keyboard_has_char()) {
                char c = keyboard_get_char();
                if (c == '1') {
                    change_state(INFO_SCREEN);
                }
            }
            break;
        }
        previous_state = current_state;
        print_main_menu();
        break;
    }

    case INFO_SCREEN: {
        if (same_state_check()) {
            if (keyboard_has_char()) {
                char c = keyboard_get_char();
                if (c == '\x1B') {
                    change_state(MENU);
                }
            }
            break;
        }
        previous_state = current_state;
        print_about_screen();
        break;
    }
    
    default:
        __asm__ volatile ("hlt");
        break;
    }
}