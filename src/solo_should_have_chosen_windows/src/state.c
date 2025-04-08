#include "state.h"
#include "main_menu/main_menu.h"

#include "start_screen/start_screen.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"


static volatile SystemState current_state = START_SCREEN;
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
    case START_SCREEN: {
        if (same_state_check()) {
            break;
        }
        previous_state = current_state;
        start_screen_reveal();
        break;
    }

    case MENU: {
        if (same_state_check()) {
            break;
        }
        previous_state = current_state;
        print_main_menu();
        break;
    }
    
    default:
        __asm__ volatile ("hlt");
        break;
    }
}