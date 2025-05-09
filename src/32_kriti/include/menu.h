// menu.h
#ifndef MENU_H
#define MENU_H

#include "libc/stdint.h"
#include "libc/stdbool.h"

#define MAX_MENU_ITEMS 20
#define MAX_TITLE_LENGTH 50
#define MAX_ITEM_LENGTH 30
#define MAX_MENU_DEPTH 5

// Forward declaration
struct Menu;

// Function pointer type for menu item actions
typedef void (*MenuAction)(void);

// Menu item types
typedef enum {
    ITEM_TYPE_ACTION,  // Executes a function when selected
    ITEM_TYPE_SUBMENU, // Opens a submenu when selected
    ITEM_TYPE_TOGGLE,  // Toggles a boolean value
    ITEM_TYPE_BACK     // Returns to parent menu
} MenuItemType;

// Menu item structure
typedef struct MenuItem {
    char text[MAX_ITEM_LENGTH];
    MenuItemType type;
    union {
        MenuAction action;           // For ITEM_TYPE_ACTION
        struct Menu* submenu;        // For ITEM_TYPE_SUBMENU
        struct {
            bool* value;             // For ITEM_TYPE_TOGGLE
            char on_text[16];        // Text when toggle is on
            char off_text[16];       // Text when toggle is off
        } toggle;
    };
    bool enabled;  // Whether this item can be selected
} MenuItem;

// Menu theme structure
typedef struct {
    uint8_t title_fg;          // Title foreground color
    uint8_t title_bg;          // Title background color
    uint8_t normal_fg;         // Normal item foreground
    uint8_t normal_bg;         // Normal item background
    uint8_t selected_fg;       // Selected item foreground
    uint8_t selected_bg;       // Selected item background
    uint8_t disabled_fg;       // Disabled item foreground
    uint8_t disabled_bg;       // Disabled item background
    uint8_t border_color;      // Border color
    bool draw_border;          // Whether to draw a border
    bool center_title;         // Whether to center the title
    char border_chars[9];      // Border characters (corners, edges)
} MenuTheme;

// Menu structure
typedef struct Menu {
    char title[MAX_TITLE_LENGTH];
    MenuItem items[MAX_MENU_ITEMS];
    int num_items;
    int selected_item;
    int is_active;
    struct Menu* parent;       // Parent menu for nested menus
    MenuTheme* theme;          // Display theme
    int x;                     // X position on screen
    int y;                     // Y position on screen
    int width;                 // Width of menu
    int height;                // Height of menu
} Menu;

// Color constants for themes
#define COLOR_BLACK     0x0
#define COLOR_BLUE      0x1
#define COLOR_GREEN     0x2
#define COLOR_CYAN      0x3
#define COLOR_RED       0x4
#define COLOR_MAGENTA   0x5
#define COLOR_BROWN     0x6
#define COLOR_LGRAY     0x7
#define COLOR_DGRAY     0x8
#define COLOR_LBLUE     0x9
#define COLOR_LGREEN    0xA
#define COLOR_LCYAN     0xB
#define COLOR_LRED      0xC
#define COLOR_LMAGENTA  0xD
#define COLOR_YELLOW    0xE
#define COLOR_WHITE     0xF

// Menu manipulation functions
void menu_init(Menu *menu, const char *title);
int menu_add_item(Menu *menu, const char *item);
int menu_add_action_item(Menu *menu, const char *item, MenuAction action);
int menu_add_submenu_item(Menu *menu, const char *item, Menu *submenu);
int menu_add_toggle_item(Menu *menu, const char *item, bool *value, const char *on_text, const char *off_text);
int menu_add_back_item(Menu *menu, const char *item);
void menu_disable_item(Menu *menu, int index);
void menu_enable_item(Menu *menu, int index);

// Menu display functions
void menu_display(Menu *menu);
void menu_clear_screen(void);
int menu_handle_input(Menu *menu, uint8_t scancode);
void menu_set_position(Menu *menu, int x, int y, int width, int height);

// Menu theme functions
MenuTheme* menu_create_theme(void);
void menu_set_theme(Menu *menu, MenuTheme *theme);
MenuTheme* menu_create_default_theme(void);
MenuTheme* menu_create_blue_theme(void);
MenuTheme* menu_create_green_theme(void);
MenuTheme* menu_create_red_theme(void);
MenuTheme* menu_create_classic_theme(void);

// Menu navigation
int menu_get_selected(Menu *menu);
uint8_t menu_wait_for_key(void);
void menu_run(Menu *menu);

// Piano keyboard mode
void piano_keyboard_mode(void);
void piano_play_note(uint16_t frequency);
void piano_stop_note(void);

#endif /* MENU_H */