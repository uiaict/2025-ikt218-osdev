#ifndef MENU_H
#define MENU_H

#include "libc/stdint.h"
#include "libc/stdbool.h"

// Menu item structure
typedef struct {
    char title[32];              // Title of the menu item
    void (*action)(void);        // Function pointer to action
    char description[64];        // Description of what this menu item does
} MenuItem;

// Menu structure
typedef struct {
    char title[32];              // Title of the menu
    MenuItem* items;             // Array of menu items
    uint8_t item_count;          // Number of items in the menu
    uint8_t selected_index;      // Currently selected item
    bool is_active;              // Whether the menu is currently active
    bool should_exit;            // Flag to exit the menu
} Menu;

// Key codes
#define KEY_UP      0x48
#define KEY_DOWN    0x50
#define KEY_LEFT    0x4B
#define KEY_RIGHT   0x4D
#define KEY_ENTER   0x1C
#define KEY_ESC     0x01

// Initialize the menu system
void menu_init();

// Create a new menu
Menu* menu_create(const char* title, uint8_t max_items);

// Add an item to a menu
void menu_add_item(Menu* menu, const char* title, void (*action)(void), const char* description);

// Display the menu
void menu_display(Menu* menu);

// Process keyboard input for the menu
void menu_process_input(Menu* menu, uint8_t scancode);

// Run the menu loop
void menu_run(Menu* menu);

// Main menu initialization
void main_menu_init();

// Run the main menu
void main_menu_run();

#endif /* MENU_H */