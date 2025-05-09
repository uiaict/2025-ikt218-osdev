// menu.c
#include "menu.h"
#include "kprint.h"
#include "keyboard.h"
#include "isr.h"
#include "pit.h"  // For timing functions
#include "screen.h"  // For screen functions

// Use extern declaration for malloc/free since we don't have stdlib.h
extern void* malloc(size_t size);
extern void free(void* ptr);

// VGA text mode constants
#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

// Default themes
static MenuTheme default_theme = {
    .title_fg = COLOR_WHITE,
    .title_bg = COLOR_BLUE,
    .normal_fg = COLOR_LGRAY,
    .normal_bg = COLOR_BLACK,
    .selected_fg = COLOR_BLACK,
    .selected_bg = COLOR_WHITE,
    .disabled_fg = COLOR_DGRAY,
    .disabled_bg = COLOR_BLACK,
    .border_color = COLOR_LGRAY,
    .draw_border = true,
    .center_title = true,
    .border_chars = {'┌', '─', '┐', '│', ' ', '│', '└', '─', '┘'}  // Using array initialization instead of string
};

static MenuTheme blue_theme = {
    .title_fg = COLOR_WHITE,
    .title_bg = COLOR_BLUE,
    .normal_fg = COLOR_WHITE,
    .normal_bg = COLOR_BLUE,
    .selected_fg = COLOR_BLUE,
    .selected_bg = COLOR_WHITE,
    .disabled_fg = COLOR_LGRAY,
    .disabled_bg = COLOR_BLUE,
    .border_color = COLOR_LCYAN,
    .draw_border = true,
    .center_title = true,
    .border_chars = {'╔', '═', '╗', '║', ' ', '║', '╚', '═', '╝'}
};

static MenuTheme green_theme = {
    .title_fg = COLOR_BLACK,
    .title_bg = COLOR_GREEN,
    .normal_fg = COLOR_LGREEN,
    .normal_bg = COLOR_BLACK,
    .selected_fg = COLOR_BLACK,
    .selected_bg = COLOR_LGREEN,
    .disabled_fg = COLOR_DGRAY,
    .disabled_bg = COLOR_BLACK,
    .border_color = COLOR_GREEN,
    .draw_border = true,
    .center_title = true,
    .border_chars = {'┏', '━', '┓', '┃', ' ', '┃', '┗', '━', '┛'}
};

static MenuTheme red_theme = {
    .title_fg = COLOR_WHITE,
    .title_bg = COLOR_RED,
    .normal_fg = COLOR_LRED,
    .normal_bg = COLOR_BLACK,
    .selected_fg = COLOR_BLACK,
    .selected_bg = COLOR_LRED,
    .disabled_fg = COLOR_DGRAY,
    .disabled_bg = COLOR_BLACK,
    .border_color = COLOR_RED,
    .draw_border = true,
    .center_title = true,
    .border_chars = {'╭', '─', '╮', '│', ' ', '│', '╰', '─', '╯'}
};

static MenuTheme classic_theme = {
    .title_fg = COLOR_BLACK,
    .title_bg = COLOR_LGRAY,
    .normal_fg = COLOR_WHITE,
    .normal_bg = COLOR_BLUE,
    .selected_fg = COLOR_BLACK,
    .selected_bg = COLOR_LGRAY,
    .disabled_fg = COLOR_DGRAY,
    .disabled_bg = COLOR_BLUE,
    .border_color = COLOR_WHITE,
    .draw_border = true,
    .center_title = true,
    .border_chars = {'+', '-', '+', '|', ' ', '|', '+', '-', '+'}
};

// Simple string function implementations
static void menu_strncpy(char *dest, const char *src, int n) {
    int i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    // Null terminate if there's space
    if (i < n) {
        dest[i] = '\0';
    }
}

static int menu_strlen(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

static int menu_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Helper function to set text at a specific position with color
static void menu_write_at(int x, int y, const char* str, unsigned char fg_color, unsigned char bg_color) {
    // Save current position and color
    int saved_x, saved_y;
    unsigned char saved_color = (COLOR_WHITE << 4) | COLOR_BLACK; // Default if you can't get current
    
    // Set position and color for menu text
    set_cursor_pos(x, y);
    set_text_color(fg_color, bg_color);
    
    // Print the string
    print_string(str);
    
    // Restore the previous cursor position and color
    // Note: You might need to add functions to get the current cursor position
    // and color if you want to fully restore the state
    set_text_color(COLOR_LGRAY, COLOR_BLACK); // Default text color
}

// Helper function to directly write to VGA memory
static void write_char_at(int x, int y, char c, unsigned char fg_color, unsigned char bg_color) {
    unsigned char* video_memory = (unsigned char*)VGA_MEMORY;
    int offset = 2 * (y * VGA_WIDTH + x);
    unsigned char attr = (bg_color << 4) | fg_color;
    
    video_memory[offset] = c;
    video_memory[offset + 1] = attr;
}

// Helper function to create a theme object
MenuTheme* menu_create_theme(void) {
    MenuTheme* theme = (MenuTheme*)malloc(sizeof(MenuTheme));
    if (theme) {
        // Initialize with default values
        *theme = default_theme;
    }
    return theme;
}

// Set a theme for a menu
void menu_set_theme(Menu *menu, MenuTheme *theme) {
    menu->theme = theme;
}

// Create and return different theme types
MenuTheme* menu_create_default_theme(void) {
    MenuTheme* theme = menu_create_theme();
    if (theme) {
        *theme = default_theme;
    }
    return theme;
}

MenuTheme* menu_create_blue_theme(void) {
    MenuTheme* theme = menu_create_theme();
    if (theme) {
        *theme = blue_theme;
    }
    return theme;
}

MenuTheme* menu_create_green_theme(void) {
    MenuTheme* theme = menu_create_theme();
    if (theme) {
        *theme = green_theme;
    }
    return theme;
}

MenuTheme* menu_create_red_theme(void) {
    MenuTheme* theme = menu_create_theme();
    if (theme) {
        *theme = red_theme;
    }
    return theme;
}

MenuTheme* menu_create_classic_theme(void) {
    MenuTheme* theme = menu_create_theme();
    if (theme) {
        *theme = classic_theme;
    }
    return theme;
}

// Clear the screen
void menu_clear_screen(void) {
    kprint("MENU: Clearing screen\n");
    
    // Set default menu text color
    set_text_color(COLOR_LGRAY, COLOR_BLACK);
    
    // Clear using the central screen function
    clear_screen();
}


// Add this new function to clear just the area for a menu
void menu_clear_area(Menu *menu) {
    kprint("MENU: Clearing menu area\n");
    
    unsigned short blank = ' ' | ((COLOR_BLACK << 4) | COLOR_LGRAY) << 8;
    unsigned short *vga_buffer = (unsigned short *)VGA_TEXT_BUFFER;
    
    // Ensure we don't try to clear outside the screen
    int max_width = (menu->x + menu->width >= SCREEN_WIDTH) ? SCREEN_WIDTH - menu->x : menu->width;
    int max_height = (menu->y + menu->height >= SCREEN_HEIGHT) ? SCREEN_HEIGHT - menu->y : menu->height;
    
    // Clear just the area where the menu will be displayed
    for (int y = menu->y; y < menu->y + max_height; y++) {
        for (int x = menu->x; x < menu->x + max_width; x++) {
            vga_buffer[y * SCREEN_WIDTH + x] = blank;
        }
    }
}

// Initialize a menu with a title
void menu_init(Menu *menu, const char *title) {
    kprint("MENU: Initializing menu with title \"");
    kprint(title);
    kprint("\"\n");
    
    menu_strncpy(menu->title, title, MAX_TITLE_LENGTH - 1);
    menu->title[MAX_TITLE_LENGTH - 1] = '\0';
    menu->num_items = 0;
    menu->selected_item = 0;
    menu->is_active = 1;
    menu->parent = NULL;
    menu->theme = &default_theme;  // Use default theme
    
    // Default position and size
    menu->x = 5;
    menu->y = 3;
    menu->width = 70;
    menu->height = 15;
}

// Set position and size of a menu
void menu_set_position(Menu *menu, int x, int y, int width, int height) {
    menu->x = x;
    menu->y = y;
    menu->width = width;
    menu->height = height;
}

// Add a simple menu item (backwards compatibility)
int menu_add_item(Menu *menu, const char *item) {
    if (menu->num_items >= MAX_MENU_ITEMS) {
        kprint("MENU: Failed to add item - menu is full\n");
        return -1; // Menu is full
    }
    
    menu_strncpy(menu->items[menu->num_items].text, item, MAX_ITEM_LENGTH - 1);
    menu->items[menu->num_items].text[MAX_ITEM_LENGTH - 1] = '\0';
    menu->items[menu->num_items].type = ITEM_TYPE_ACTION;
    menu->items[menu->num_items].action = NULL;  // No action
    menu->items[menu->num_items].enabled = true;
    
    kprint("MENU: Added item \"");
    kprint(menu->items[menu->num_items].text);
    kprint("\" at index ");
    kprint_dec(menu->num_items);
    kprint("\n");
    
    menu->num_items++;
    return menu->num_items - 1; // Return index of added item
}

// Add an action item
int menu_add_action_item(Menu *menu, const char *item, MenuAction action) {
    if (menu->num_items >= MAX_MENU_ITEMS) {
        kprint("MENU: Failed to add action item - menu is full\n");
        return -1; // Menu is full
    }
    
    menu_strncpy(menu->items[menu->num_items].text, item, MAX_ITEM_LENGTH - 1);
    menu->items[menu->num_items].text[MAX_ITEM_LENGTH - 1] = '\0';
    menu->items[menu->num_items].type = ITEM_TYPE_ACTION;
    menu->items[menu->num_items].action = action;
    menu->items[menu->num_items].enabled = true;
    
    kprint("MENU: Added action item \"");
    kprint(menu->items[menu->num_items].text);
    kprint("\" at index ");
    kprint_dec(menu->num_items);
    kprint("\n");
    
    menu->num_items++;
    return menu->num_items - 1; // Return index of added item
}

// Add a submenu item
int menu_add_submenu_item(Menu *menu, const char *item, Menu *submenu) {
    if (menu->num_items >= MAX_MENU_ITEMS) {
        kprint("MENU: Failed to add submenu item - menu is full\n");
        return -1; // Menu is full
    }
    
    menu_strncpy(menu->items[menu->num_items].text, item, MAX_ITEM_LENGTH - 1);
    menu->items[menu->num_items].text[MAX_ITEM_LENGTH - 1] = '\0';
    menu->items[menu->num_items].type = ITEM_TYPE_SUBMENU;
    menu->items[menu->num_items].submenu = submenu;
    menu->items[menu->num_items].enabled = true;
    
    // Set parent reference for the submenu
    submenu->parent = menu;
    
    kprint("MENU: Added submenu item \"");
    kprint(menu->items[menu->num_items].text);
    kprint("\" at index ");
    kprint_dec(menu->num_items);
    kprint("\n");
    
    menu->num_items++;
    return menu->num_items - 1; // Return index of added item
}

// Add a toggle item
int menu_add_toggle_item(Menu *menu, const char *item, bool *value, const char *on_text, const char *off_text) {
    if (menu->num_items >= MAX_MENU_ITEMS) {
        kprint("MENU: Failed to add toggle item - menu is full\n");
        return -1; // Menu is full
    }
    
    menu_strncpy(menu->items[menu->num_items].text, item, MAX_ITEM_LENGTH - 1);
    menu->items[menu->num_items].text[MAX_ITEM_LENGTH - 1] = '\0';
    menu->items[menu->num_items].type = ITEM_TYPE_TOGGLE;
    menu->items[menu->num_items].toggle.value = value;
    menu_strncpy(menu->items[menu->num_items].toggle.on_text, on_text, 15);
    menu->items[menu->num_items].toggle.on_text[15] = '\0';
    menu_strncpy(menu->items[menu->num_items].toggle.off_text, off_text, 15);
    menu->items[menu->num_items].toggle.off_text[15] = '\0';
    menu->items[menu->num_items].enabled = true;
    
    kprint("MENU: Added toggle item \"");
    kprint(menu->items[menu->num_items].text);
    kprint("\" at index ");
    kprint_dec(menu->num_items);
    kprint("\n");
    
    menu->num_items++;
    return menu->num_items - 1; // Return index of added item
}

// Add a back item
int menu_add_back_item(Menu *menu, const char *item) {
    if (menu->num_items >= MAX_MENU_ITEMS) {
        kprint("MENU: Failed to add back item - menu is full\n");
        return -1; // Menu is full
    }
    
    menu_strncpy(menu->items[menu->num_items].text, item, MAX_ITEM_LENGTH - 1);
    menu->items[menu->num_items].text[MAX_ITEM_LENGTH - 1] = '\0';
    menu->items[menu->num_items].type = ITEM_TYPE_BACK;
    menu->items[menu->num_items].enabled = true;
    
    /*kprint("MENU: Added back item \"");
    kprint(menu->items[menu->num_items].text);
    kprint("\" at index ");
    kprint_dec(menu->num_items);
    kprint("\n");*/

    
    menu->num_items++;
    return menu->num_items - 1; // Return index of added item
}

// Disable a menu item
void menu_disable_item(Menu *menu, int index) {
    if (index >= 0 && index < menu->num_items) {
        menu->items[index].enabled = false;
    }
}

// Enable a menu item
void menu_enable_item(Menu *menu, int index) {
    if (index >= 0 && index < menu->num_items) {
        menu->items[index].enabled = true;
    }
}

// Display the menu
void menu_display(Menu *menu) {
    //kprint("MENU: Displaying menu\n");
    
    MenuTheme *theme = menu->theme;
    
    // Ensure we don't try to draw outside the screen
    int max_width = (menu->x + menu->width >= VGA_WIDTH) ? VGA_WIDTH - menu->x - 1 : menu->width;
    int max_height = (menu->y + menu->height >= VGA_HEIGHT) ? VGA_HEIGHT - menu->y - 1 : menu->height;
    
    // Draw border if requested
    if (theme->draw_border) {
        // Top border
        write_char_at(menu->x, menu->y, theme->border_chars[0], theme->border_color, COLOR_BLACK); // Top-left
        for (int i = 1; i < max_width - 1; i++) {
            write_char_at(menu->x + i, menu->y, theme->border_chars[1], theme->border_color, COLOR_BLACK); // Top
        }
        write_char_at(menu->x + max_width - 1, menu->y, theme->border_chars[2], theme->border_color, COLOR_BLACK); // Top-right
        
        // Side borders
        for (int i = 1; i < max_height - 1; i++) {
            write_char_at(menu->x, menu->y + i, theme->border_chars[3], theme->border_color, COLOR_BLACK); // Left
            write_char_at(menu->x + max_width - 1, menu->y + i, theme->border_chars[5], theme->border_color, COLOR_BLACK); // Right
        }
        
        // Bottom border
        write_char_at(menu->x, menu->y + max_height - 1, theme->border_chars[6], theme->border_color, COLOR_BLACK); // Bottom-left
        for (int i = 1; i < max_width - 1; i++) {
            write_char_at(menu->x + i, menu->y + max_height - 1, theme->border_chars[7], theme->border_color, COLOR_BLACK); // Bottom
        }
        write_char_at(menu->x + max_width - 1, menu->y + max_height - 1, theme->border_chars[8], theme->border_color, COLOR_BLACK); // Bottom-right
    }
    
    // Clear the inside of the menu (fill with spaces)
    for (int y = menu->y + 1; y < menu->y + max_height - 1; y++) {
        for (int x = menu->x + 1; x < menu->x + max_width - 1; x++) {
            write_char_at(x, y, ' ', theme->normal_fg, theme->normal_bg);
        }
    }
    
    // Calculate title position
    int title_x = menu->x + 1;
    if (theme->center_title) {
        int title_len = menu_strlen(menu->title);
        title_x = menu->x + (max_width - title_len) / 2;
        if (title_x < menu->x + 1) title_x = menu->x + 1;
    }
    
    // Display title
    menu_write_at(title_x, menu->y + 1, menu->title, theme->title_fg, theme->title_bg);
    
    // Display menu items
    int y_offset = menu->y + 3; // Start items 2 lines below title
    
    for (int i = 0; i < menu->num_items && y_offset < menu->y + max_height - 1; i++) {
        char display_text[MAX_ITEM_LENGTH + 10]; // Extra space for toggle status or submenu indicator
        
        // Prepare display text based on item type
        switch (menu->items[i].type) {
            case ITEM_TYPE_SUBMENU:
                menu_strncpy(display_text, menu->items[i].text, MAX_ITEM_LENGTH - 3);
                display_text[MAX_ITEM_LENGTH - 3] = '\0';
                menu_strncpy(display_text + menu_strlen(display_text), " >", 3);
                break;
                
            case ITEM_TYPE_TOGGLE:
                menu_strncpy(display_text, menu->items[i].text, MAX_ITEM_LENGTH - 20);
                display_text[MAX_ITEM_LENGTH - 20] = '\0';
                menu_strncpy(display_text + menu_strlen(display_text), ": ", 3);
                if (*(menu->items[i].toggle.value)) {
                    menu_strncpy(display_text + menu_strlen(display_text), menu->items[i].toggle.on_text, 16);
                } else {
                    menu_strncpy(display_text + menu_strlen(display_text), menu->items[i].toggle.off_text, 16);
                }
                break;
                
            case ITEM_TYPE_BACK:
                menu_strncpy(display_text, "< ", 3);
                menu_strncpy(display_text + 2, menu->items[i].text, MAX_ITEM_LENGTH - 3);
                display_text[MAX_ITEM_LENGTH - 1] = '\0';
                break;
                
            default: // ITEM_TYPE_ACTION or others
                menu_strncpy(display_text, menu->items[i].text, MAX_ITEM_LENGTH);
                display_text[MAX_ITEM_LENGTH - 1] = '\0';
                break;
        }
        
        // Determine colors based on selection and enabled state
        unsigned char fg, bg;
        if (!menu->items[i].enabled) {
            fg = theme->disabled_fg;
            bg = theme->disabled_bg;
        } else if (i == menu->selected_item) {
            fg = theme->selected_fg;
            bg = theme->selected_bg;
        } else {
            fg = theme->normal_fg;
            bg = theme->normal_bg;
        }
        
        // Draw selection indicator
        if (i == menu->selected_item && menu->items[i].enabled) {
            menu_write_at(menu->x + 2, y_offset, ">", fg, bg);
        } else {
            menu_write_at(menu->x + 2, y_offset, " ", fg, bg);
        }
        
        // Draw the menu item text
        menu_write_at(menu->x + 4, y_offset, display_text, fg, bg);
        
        y_offset++;
    }
    
    // Display instructions at the bottom
    menu_write_at(menu->x + 1, menu->y + max_height - 2, 
                 "↑/↓: Navigate  Enter: Select  Esc: Back", 
                 theme->normal_fg, theme->normal_bg);
}

// Handle keyboard input for menu navigation
int menu_handle_input(Menu *menu, uint8_t scancode) {
    /*kprint("MENU: Handling input scancode 0x");
    kprint_hex(scancode);
    kprint("\n");*/
    
    // Handle the key based on its scancode
    switch (scancode) {
        case SCANCODE_UP:
           // kprint("MENU: UP arrow key\n");
            
            // Find the previous enabled item
            int prev = menu->selected_item;
            do {
                prev = (prev > 0) ? prev - 1 : menu->num_items - 1;
                if (prev == menu->selected_item) break; // Full loop, no enabled items
            } while (!menu->items[prev].enabled);
            
            if (menu->items[prev].enabled) {
                menu->selected_item = prev;
                /*kprint("MENU: Selection moved up to ");
                kprint_dec(menu->selected_item);
                kprint("\n");*/
            } else {
                kprint("MENU: No enabled items to select\n");
            }
            
            return 0; // Menu changed, not selected
            
        case SCANCODE_DOWN:
            //kprint("MENU: DOWN arrow key\n");
            
            // Find the next enabled item
            int next = menu->selected_item;
            do {
                next = (next < menu->num_items - 1) ? next + 1 : 0;
                if (next == menu->selected_item) break; // Full loop, no enabled items
            } while (!menu->items[next].enabled);
            
            if (menu->items[next].enabled) {
                menu->selected_item = next;
                /*kprint("MENU: Selection moved down to ");
                kprint_dec(menu->selected_item);
                kprint("\n");*/
            } else {
                kprint("MENU: No enabled items to select\n");
            }
            
            return 0; // Menu changed, not selected
            
        case SCANCODE_ENTER:
           /* kprint("MENU: ENTER key - item ");
            kprint_dec(menu->selected_item);
            kprint(" selected\n");*/
            
            // Check if the selected item is enabled
            if (!menu->items[menu->selected_item].enabled) {
                //kprint("MENU: Selected item is disabled\n");
                return 0;
            }
            
            // Handle the selection based on item type
            switch (menu->items[menu->selected_item].type) {
                case ITEM_TYPE_ACTION:
                    if (menu->items[menu->selected_item].action) {
                        //kprint("MENU: Executing action for item\n");
                        menu->items[menu->selected_item].action();
                    }
                    return 1; // Item selected
                    
                case ITEM_TYPE_SUBMENU:
                    if (menu->items[menu->selected_item].submenu) {
                       // kprint("MENU: Opening submenu\n");
                        Menu* submenu = menu->items[menu->selected_item].submenu;
                        menu_run(submenu);
                        return 0; // Stay in current menu after submenu closes
                    }
                    break;
                    
                case ITEM_TYPE_TOGGLE:
                    if (menu->items[menu->selected_item].toggle.value) {
                        bool* value = menu->items[menu->selected_item].toggle.value;
                        *value = !(*value); // Toggle the value
                        kprint("MENU: Toggled value to ");
                        kprint(*value ? "ON" : "OFF");
                        kprint("\n");
                    }
                    return 0; // Stay in menu after toggle
                    
                case ITEM_TYPE_BACK:
                    //kprint("MENU: Back item selected\n");
                    menu->is_active = 0; // Exit this menu
                    return -1; // Exit menu
            }
            
            return 1; // Default to item selected
            
        case SCANCODE_ESC:
            //kprint("MENU: ESC key - exiting menu\n");
            
            menu->is_active = 0;
            return -1; // Exit menu
            
        case SCANCODE_LEFT:
            // Left key - handle as back if in submenu
            if (menu->parent != NULL) {
                kprint("MENU: LEFT key - going back to parent menu\n");
                menu->is_active = 0;
                return -1; // Exit menu to go back to parent
            }
            break;
            
        case SCANCODE_RIGHT:
            // Right key - handle as enter for submenus
            if (menu->items[menu->selected_item].type == ITEM_TYPE_SUBMENU && 
                menu->items[menu->selected_item].enabled) {
                kprint("MENU: RIGHT key - opening submenu\n");
                Menu* submenu = menu->items[menu->selected_item].submenu;
                menu_run(submenu);
                return 0; // Stay in current menu after submenu closes
            }
            break;
            
        default:
            /*kprint("MENU: Unhandled key 0x");
            kprint_hex(scancode);
            kprint("\n");*/
            break;
    }
    
    return 0; // No selection made
}

// Get the currently selected item index
int menu_get_selected(Menu *menu) {
    return menu->selected_item;
}

// Wait for a key press and return the scancode
uint8_t menu_wait_for_key(void) {
   // kprint("MENU: Waiting for key press...\n");
    
    // Implementation uses keyboard_wait_for_key from keyboard.c
    uint8_t scancode = keyboard_wait_for_key();
    
    /*kprint("MENU: Got key scancode 0x");
    kprint_hex(scancode);
    kprint("\n");*/
    
    return scancode;
}

// Run a menu until the user makes a selection or exits
void menu_run(Menu *menu) {
    menu_clear_screen();
    // Mark menu as active
    menu->is_active = 1;
    
    // Initial display
    menu_display(menu);
    
    // Debug output
    kprint("Menu running: ");
    kprint(menu->title);
    kprint("\n");
    
    // Main loop
    while (menu->is_active) {
        // Wait for input
        uint8_t scancode = menu_wait_for_key();
        
        // Debug output
        /*kprint("Key received: 0x");
        kprint_hex(scancode);
        kprint("\n");*/
        
        // Handle input
        int result = menu_handle_input(menu, scancode);
        
        // Debug output
        /*kprint("Menu result: ");
        kprint_dec(result);
        kprint(", Selected: ");
        kprint_dec(menu->selected_item);
        kprint("\n");*/
        
        // Redisplay menu
        menu_display(menu);
        
        // Check if menu should exit
        if (result == 1) { // Item selected with Enter
            /*kprint("Item selected: ");
            kprint_dec(menu->selected_item);
            kprint("\n");*/
            break; // Exit the loop
        } else if (result == -1) { // ESC pressed
            kprint("Menu exited with ESC\n");
            break; // Exit the loop
        }
    }
}

// Piano keyboard constants
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523

// Helper function to draw piano keyboard
static void draw_piano_keyboard(bool highlighting[]) {
    int x_start = 5;
    int y_start = 5;
    
    // Draw white keys
    for (int i = 0; i < 8; i++) {
        unsigned char color = highlighting[i*2] ? COLOR_LGREEN : COLOR_WHITE;
        
        // Draw key outline
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 7; x++) {
                if (x == 0 || x == 6 || y == 0 || y == 9) {
                    write_char_at(x_start + i*8 + x, y_start + y, ' ', COLOR_BLACK, COLOR_BLACK);
                } else {
                    write_char_at(x_start + i*8 + x, y_start + y, ' ', COLOR_BLACK, color);
                }
            }
        }
        
        // Add key label (A through G)
        char key_label = 'A' + (i % 7);
        write_char_at(x_start + i*8 + 3, y_start + 7, key_label, COLOR_BLACK, color);
    }
    
    // Draw black keys
    int black_key_positions[] = {0, 1, 3, 4, 5}; // Positions of black keys in octave
    for (int i = 0; i < 5; i++) {
        int pos = black_key_positions[i];
        unsigned char color = highlighting[pos*2+1] ? COLOR_LGREEN : COLOR_BLACK;
        
        // Draw key outline
        for (int y = 0; y < 6; y++) {
            for (int x = 0; x < 5; x++) {
                write_char_at(x_start + pos*8 + 5 + x, y_start + y, ' ', COLOR_WHITE, color);
            }
        }
    }
    
    // Draw key mappings
    menu_write_at(x_start, y_start + 12, "Piano Keyboard Controls:", COLOR_LGRAY, COLOR_BLACK);
    menu_write_at(x_start, y_start + 14, "White keys: Z X C V B N M ,   (A through G)", COLOR_LGRAY, COLOR_BLACK);
    menu_write_at(x_start, y_start + 15, "Black keys: S D   G H J     (Sharp notes)", COLOR_LGRAY, COLOR_BLACK);
    menu_write_at(x_start, y_start + 17, "Press ESC to return to menu", COLOR_LGRAY, COLOR_BLACK);
}

// Function to play a musical note through the PC speaker
void piano_play_note(uint16_t frequency) {
    if (frequency == 0) {
        // Silence
        outb(0x61, inb(0x61) & ~3);  // Disable speaker
        return;
    }
    
    // Set up frequency
    uint32_t divisor = 1193180 / frequency;
    outb(0x43, 0xB6);                // Command byte: channel 2, mode 3, binary
    outb(0x42, divisor & 0xFF);      // Low byte of divisor
    outb(0x42, (divisor >> 8) & 0xFF); // High byte of divisor
    
    // Enable speaker
    outb(0x61, inb(0x61) | 3);       // Connect speaker to PIT channel 2
}

// Function to stop playing a note
void piano_stop_note(void) {
    outb(0x61, inb(0x61) & ~3);      // Disable speaker
}

// Keyboard piano mode
void piano_keyboard_mode(void) {
    // Clear the screen
    menu_clear_screen();
    
    // Display title
    menu_write_at(10, 1, "KEYBOARD PIANO MODE", COLOR_WHITE, COLOR_BLUE);
    
    // Initial state: no keys are highlighted
    bool key_highlighting[16] = {false};
    
    // Draw the piano keyboard
    draw_piano_keyboard(key_highlighting);
    
    kprint("Piano mode active. Press keys to play notes.\n");
    kprint("Press ESC to exit piano mode.\n");
    
    // Main piano loop
    bool running = true;
    while (running) {
        // Check if a key is pressed
        if (keyboard_data_available()) {
            uint8_t scancode = keyboard_get_scancode();
            
            /*kprint("Piano key pressed: 0x");
            kprint_hex(scancode);
            kprint("\n");*/
            
            // Reset all highlighting
            for (int i = 0; i < 16; i++) {
                key_highlighting[i] = false;
            }
            
            // Check if it's a key release (bit 7 set)
            if (scancode & 0x80) {
                // Key released - stop playing sound
                piano_stop_note();
            } else {
                // Determine which note to play based on the key
                uint16_t frequency = 0;
                int highlight_idx = -1;
                
                switch (scancode) {
                    // White keys (ZXCVBNM,)
                    case SCANCODE_Z:  // C
                        frequency = NOTE_C4;
                        highlight_idx = 0;
                        break;
                    case SCANCODE_X:  // D
                        frequency = NOTE_D4;
                        highlight_idx = 2;
                        break;
                    case SCANCODE_C:  // E
                        frequency = NOTE_E4;
                        highlight_idx = 4;
                        break;
                    case SCANCODE_V:  // F
                        frequency = NOTE_F4;
                        highlight_idx = 6;
                        break;
                    case SCANCODE_B:  // G
                        frequency = NOTE_G4;
                        highlight_idx = 8;
                        break;
                    case SCANCODE_N:  // A
                        frequency = NOTE_A4;
                        highlight_idx = 10;
                        break;
                    case SCANCODE_M:  // B
                        frequency = NOTE_B4;
                        highlight_idx = 12;
                        break;
                    case SCANCODE_COMMA:  // C5
                        frequency = NOTE_C5;
                        highlight_idx = 14;
                        break;
                        
                    // Black keys (SDFGHJ)
                    case SCANCODE_S:  // C#
                        frequency = NOTE_CS4;
                        highlight_idx = 1;
                        break;
                    case SCANCODE_D:  // D#
                        frequency = NOTE_DS4;
                        highlight_idx = 3;
                        break;
                    case SCANCODE_G:  // F#
                        frequency = NOTE_FS4;
                        highlight_idx = 7;
                        break;
                    case SCANCODE_H:  // G#
                        frequency = NOTE_GS4;
                        highlight_idx = 9;
                        break;
                    case SCANCODE_J:  // A#
                        frequency = NOTE_AS4;
                        highlight_idx = 11;
                        break;
                        
                    case SCANCODE_ESC:  // Exit piano mode
                        running = false;
                        piano_stop_note();
                        break;
                        
                    default:
                        frequency = 0;  // No sound for other keys
                        break;
                }
                
                // Play the note
                if (frequency > 0) {
                    piano_play_note(frequency);
                    
                    // Set highlighting
                    if (highlight_idx >= 0 && highlight_idx < 16) {
                        key_highlighting[highlight_idx] = true;
                    }
                }
            }
            
            // Update the keyboard display with highlighting
            draw_piano_keyboard(key_highlighting);
        }
    }
    
    // Make sure the speaker is off when we exit
    piano_stop_note();
    
    // Clear the screen before returning
    menu_clear_screen();
}