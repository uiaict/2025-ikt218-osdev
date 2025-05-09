#pragma once

#include <libc/stdint.h>
#include <libc/stdbool.h>
#include <libc/stddef.h>
#include <driver/include/keyboard.h>

////////////////////////////////////////
// Configuration
////////////////////////////////////////

#define MAX_MENU_ITEMS         10
#define MAX_MENU_TITLE_LENGTH  40
#define MAX_MENU_ITEM_LENGTH   30

////////////////////////////////////////
// Menu Item
////////////////////////////////////////

typedef struct {
    char        title[MAX_MENU_ITEM_LENGTH];
    void      (*action)(void);
} MenuItem;

////////////////////////////////////////
// Menu Structure
////////////////////////////////////////

typedef struct {
    char      title[MAX_MENU_TITLE_LENGTH];
    MenuItem  items[MAX_MENU_ITEMS];
    size_t    item_count;
    size_t    selected_index;
    bool      running;
} Menu;

////////////////////////////////////////
// Menu API
////////////////////////////////////////

void menu_init         (Menu* menu, const char* title);
void menu_add_item     (Menu* menu, const char* title, void (*action)(void));
void menu_render       (Menu* menu);
void menu_handle_input (Menu* menu, KeyCode key);
void menu_run          (Menu* menu);
void menu_exit         (Menu* menu);
void clear_keyboard_buffer(void);
