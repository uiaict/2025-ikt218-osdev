// src/ui/menu.c

#include <libc/stdio.h>
#include <libc/stddef.h>
#include <ui/ui_common.h>
#include <ui/menu.h>
#include <driver/include/keyboard.h>

////////////////////////////////////////
// External Keyboard Input Functions
////////////////////////////////////////

extern KeyCode keyboard_get_key(void);
extern bool keyboard_buffer_empty(void);
extern uint8_t keyboard_buffer_dequeue(void);

////////////////////////////////////////
// Input Buffer Utility
////////////////////////////////////////

// Clear all pending keyboard input
void clear_keyboard_buffer(void) {
    while (!keyboard_buffer_empty()) {
        keyboard_buffer_dequeue();
    }
}

////////////////////////////////////////
// Menu Initialization
////////////////////////////////////////

// Initialize menu structure with title
void menu_init(Menu* menu, const char* title) {
    size_t i;
    for (i = 0; title[i] && i + 1 < MAX_MENU_TITLE_LENGTH; i++) {
        menu->title[i] = title[i];
    }
    menu->title[i] = '\0';
    menu->item_count     = 0;
    menu->selected_index = 0;
    menu->running        = false;
}

// Add a menu item (title + function pointer)
void menu_add_item(Menu* menu, const char* title, void (*action)(void)) {
    if (menu->item_count >= MAX_MENU_ITEMS) return;
    MenuItem* item = &menu->items[menu->item_count++];
    size_t j;
    for (j = 0; title[j] && j + 1 < MAX_MENU_ITEM_LENGTH; j++) {
        item->title[j] = title[j];
    }
    item->title[j] = '\0';
    item->action   = action;
}

////////////////////////////////////////
// Menu Rendering
////////////////////////////////////////

// Draw the full menu to the terminal
void menu_render(Menu* menu) {
    clear_screen();

    // Center and print menu title
    size_t len = terminal_strlen(menu->title);
    terminal_setcursor(40 - (int)(len / 2), 1);
    terminal_write(menu->title);

    // Print menu items
    for (size_t i = 0; i < menu->item_count; i++) {
        terminal_setcursor(20, i + 4);
        if (i == menu->selected_index) {
            terminal_write("> ");
            uint8_t old = terminal_getcolor();
            terminal_setcolor(0x0F);  // Highlight
            terminal_write(menu->items[i].title);
            terminal_setcolor(old);
        } else {
            terminal_write("  ");
            terminal_write(menu->items[i].title);
        }
    }

    // Navigation instructions
    terminal_setcursor(10, 20);
    terminal_write("Use UP/DOWN arrows, ENTER to select, ESC to exit");
}

////////////////////////////////////////
// Input Handling
////////////////////////////////////////

// Respond to user input for menu navigation
void menu_handle_input(Menu* menu, KeyCode key) {
    switch (key) {
        case KEY_UP:
            if (menu->selected_index > 0) {
                menu->selected_index--;
            }
            break;

        case KEY_DOWN:
            if (menu->selected_index + 1 < menu->item_count) {
                menu->selected_index++;
            }
            break;

        case KEY_ENTER:
            if (menu->selected_index < menu->item_count) {
                void (*act)(void) = menu->items[menu->selected_index].action;
                if (act) {
                    clear_keyboard_buffer();
                    act();
                }
            }
            break;

        case KEY_ESC:
            menu_exit(menu);
            break;

        default:
            if (key >= '1' && key < (char)('1' + menu->item_count)) {
                size_t idx = (size_t)(key - '1');
                menu->selected_index = idx;
                void (*act)(void) = menu->items[idx].action;
                if (act) {
                    clear_keyboard_buffer();
                    act();
                }
            }
            break;
    }
}

////////////////////////////////////////
// Menu Loop Control
////////////////////////////////////////

// Main menu execution loop
void menu_run(Menu* menu) {
    menu->running = true;
    while (menu->running) {
        menu_render(menu);
        KeyCode k = keyboard_get_key();
        menu_handle_input(menu, k);
    }
}

// Stop the menu loop
void menu_exit(Menu* menu) {
    menu->running = false;
}
