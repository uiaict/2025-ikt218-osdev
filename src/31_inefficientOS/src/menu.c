#include "menu.h"
#include "terminal.h"
#include "keyboard.h"
#include "memory.h"
#include "pit.h"
#include "song.h"
#include "common.h"
#include "snake.h"

// Forward declarations for functions defined in kernel.c
void uint_to_hex(uint32_t num, char* str);
void int_to_str(int num, char* str);

// Global menu objects
static Menu* main_menu = NULL;

// Forward declarations for menu actions
static void action_system_info();
static void action_memory_test();
static void action_interrupt_test();
static void action_pit_test();
static void action_music_player();
static void action_hello_world();
static void action_snake_game();

// Initialize the menu system
void menu_init() {
    // Initialize main menu
    main_menu_init();
}

// Create a new menu
Menu* menu_create(const char* title, uint8_t max_items) {
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    if (!menu) return NULL;
    
    // Copy title
    uint8_t i = 0;
    while (title[i] && i < 31) {
        menu->title[i] = title[i];
        i++;
    }
    menu->title[i] = '\0';
    
    // Allocate memory for items
    menu->items = (MenuItem*)malloc(sizeof(MenuItem) * max_items);
    if (!menu->items) {
        free(menu);
        return NULL;
    }
    
    menu->item_count = 0;
    menu->selected_index = 0;
    menu->is_active = false;
    menu->should_exit = false;
    
    return menu;
}

// Add an item to a menu
void menu_add_item(Menu* menu, const char* title, void (*action)(void), const char* description) {
    MenuItem* item = &menu->items[menu->item_count];
    
    // Copy title
    uint8_t i = 0;
    while (title[i] && i < 31) {
        item->title[i] = title[i];
        i++;
    }
    item->title[i] = '\0';
    
    // Copy description
    i = 0;
    while (description[i] && i < 63) {
        item->description[i] = description[i];
        i++;
    }
    item->description[i] = '\0';
    
    // Set action
    item->action = action;
    
    // Increment count
    menu->item_count++;
}

// Clear the terminal and display the menu
void menu_display(Menu* menu) {
    // Clear the terminal
    terminal_initialize();
    
    // Display the title
    terminal_write_colored("\n  ===== ", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    terminal_write_colored(menu->title, VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_write_colored(" =====\n\n", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    
    // Add Hello World message right after the title
    terminal_write_colored("  Hello World! Welcome to the OS menu.\n\n", 
                          VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    
    // Display items
    for (uint8_t i = 0; i < menu->item_count; i++) {
        if (i == menu->selected_index) {
            // Selected item
            terminal_write_colored("  > ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
            terminal_write_colored(menu->items[i].title, VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
            terminal_writestring("\n");
            
            // Display description
            terminal_write_colored("      ", VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
            terminal_write_colored(menu->items[i].description, VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
            terminal_writestring("\n\n");
        } else {
            // Unselected item
            terminal_writestring("    ");
            terminal_writestring(menu->items[i].title);
            terminal_writestring("\n\n");
        }
    }
    
    // Display navigation help
    terminal_write_colored("\n  Controls: Arrow Up/Down - Navigate, Enter - Select, ESC - Exit\n", 
                          VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);
}

// Process keyboard input for the menu
void menu_process_input(Menu* menu, uint8_t scancode) {
    switch (scancode) {
        case KEY_UP:
            if (menu->selected_index > 0) {
                menu->selected_index--;
                menu_display(menu);
            }
            break;
            
        case KEY_DOWN:
            if (menu->selected_index < menu->item_count - 1) {
                menu->selected_index++;
                menu_display(menu);
            }
            break;
            
        case KEY_ENTER:
            if (menu->items[menu->selected_index].action) {
                terminal_initialize(); // Clear screen before action
                menu->items[menu->selected_index].action();
            }
            break;
            
        case KEY_ESC:
            menu->should_exit = true;
            break;
    }
}

// Run the menu loop
void menu_run(Menu* menu) {
    uint8_t scancode;
    
    menu->is_active = true;
    menu->should_exit = false;
    menu_display(menu);
    
    while (menu->is_active && !menu->should_exit) {
        scancode = keyboard_get_scancode();
        if (scancode != 0) {
            menu_process_input(menu, scancode);
        }
        
        // Small delay to prevent CPU hogging
        sleep_busy(10);
    }
}

// Initialize the main menu
void main_menu_init() {
    main_menu = menu_create("31_inefficientOS", 8);
    
    menu_add_item(main_menu, "Hello World", action_hello_world, 
                 "Display the traditional Hello World message");
    menu_add_item(main_menu, "System Information", action_system_info, 
                 "Display information about the OS and hardware");
    menu_add_item(main_menu, "Interrupt Test", action_interrupt_test, 
                 "Test interrupt service routines (ISRs)");
    menu_add_item(main_menu, "Memory Management Test", action_memory_test, 
                 "Test memory allocation and paging functionality");
    menu_add_item(main_menu, "PIT Testing", action_pit_test, 
                 "Test Programmable Interval Timer (PIT) functionality");
    menu_add_item(main_menu, "Music Player", action_music_player, 
                 "Play various tunes using the PC speaker");
    menu_add_item(main_menu, "Snake Game", action_snake_game, 
                 "Play the classic Snake game");
    menu_add_item(main_menu, "Exit", NULL, 
                 "Return to default OS operation");
}

// Run the main menu
void main_menu_run() {
    menu_run(main_menu);
}

// ---- Menu Actions ----

// Action for Hello World
static void action_hello_world() {
    // Clear the screen and display a colorful Hello World message
    terminal_initialize();
    
    terminal_write_colored("===== Hello World Demo =====\n\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    
    // Display Hello World in multiple colors like in the original kernel.c
    terminal_write_colored("Hello?\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    terminal_write_colored("Hello\n", VGA_COLOR_RED, VGA_COLOR_BLACK);
    terminal_write_colored("Hello\n", VGA_COLOR_BROWN, VGA_COLOR_BLACK);
    terminal_write_colored("Hello...\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
    terminal_write_colored("Is there anybody in there?\n", VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_write_colored("Just nod if you can hear me\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    terminal_write_colored("Is there anyone home?\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    terminal_writestring("Hello world\n");
    
    // Wait for user input to return
    terminal_write_colored("\n\nPress ESC to return to main menu...\n", 
                         VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);
                         
    uint8_t scancode;
    while (1) {
        scancode = keyboard_get_scancode();
        if (scancode == KEY_ESC) {
            break;
        }
        sleep_busy(10);
    }
    
    menu_display(main_menu);
}

// Action for system information
static void action_system_info() {
    terminal_write_colored("===== System Information =====\n\n", VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    
    // Display basic system info
    terminal_writestring("OS Name: 31_inefficientOS\n");
    terminal_writestring("Version: 0.1\n");
    terminal_writestring("Architecture: x86\n");
    terminal_writestring("Features:\n");
    terminal_writestring(" - Terminal Output\n");
    terminal_writestring(" - GDT/IDT/IRQ\n");
    terminal_writestring(" - Keyboard Input\n");
    terminal_writestring(" - Memory Management\n");
    terminal_writestring(" - Programmable Interval Timer\n");
    terminal_writestring(" - PC Speaker Music\n");
    terminal_writestring(" - Snake Game\n");
    
    // Wait for user input to return
    terminal_write_colored("\n\nPress ESC to return to main menu...\n", 
                         VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);
                         
    uint8_t scancode;
    while (1) {
        scancode = keyboard_get_scancode();
        if (scancode == KEY_ESC) {
            break;
        }
        sleep_busy(10);
    }
    
    menu_display(main_menu);
}

// Action for interrupt testing
static void action_interrupt_test() {
    terminal_write_colored("===== Interrupt Service Routine Testing =====\n\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    
    terminal_writestring("This test will trigger interrupts 0, 1, and 2.\n");
    terminal_writestring("If properly implemented, each should display the correct exception message.\n\n");
    
    terminal_write_colored("Press 1, 2, or 3 to trigger different interrupts:\n", 
                          VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    terminal_writestring("1. Interrupt 0 \n");
    terminal_writestring("2. Interrupt 1 \n");
    terminal_writestring("3. Interrupt 2 \n\n");
    
    uint8_t scancode;
    while (1) {
        scancode = keyboard_get_scancode();
        
        // Check for ESC to return to main menu
        if (scancode == KEY_ESC) {
            break;
        }
        
        // Check for number keys
        char c = keyboard_scancode_to_ascii(scancode);
        if (c == '1') {
            terminal_writestring("\nTriggering interrupt 0...\n");
            asm volatile("int $0");
            terminal_writestring("\n");
        } else if (c == '2') {
            terminal_writestring("\nTriggering interrupt 1...\n");
            asm volatile("int $1");
            terminal_writestring("\n");
        } else if (c == '3') {
            terminal_writestring("\nTriggering interrupt 2...\n");
            asm volatile("int $2");
            terminal_writestring("\n");
        }
        
        // Small delay
        sleep_busy(10);
    }
    
    // Return to main menu
    menu_display(main_menu);
}

// Action for memory test
static void action_memory_test() {
    terminal_write_colored("===== Memory Management Test =====\n\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    
    // Test memory allocation
    terminal_writestring("Testing memory allocation...\n");
    
    void* memory1 = malloc(12345);
    void* memory2 = malloc(54321);
    void* memory3 = malloc(13331);
    
    char hex_str[9];
    
    terminal_writestring("Memory allocated at: ");
    uint_to_hex((uint32_t)memory1, hex_str);
    terminal_writestring(hex_str);
    terminal_writestring(", ");
    uint_to_hex((uint32_t)memory2, hex_str);
    terminal_writestring(hex_str);
    terminal_writestring(", ");
    uint_to_hex((uint32_t)memory3, hex_str);
    terminal_writestring(hex_str);
    terminal_writestring("\n\n");
    
    terminal_writestring("Memory layout:\n");
    print_memory_layout();
    
    terminal_writestring("\nFree memory test: freeing and reallocating...\n");
    free(memory2);
    memory2 = malloc(1000);
    uint_to_hex((uint32_t)memory2, hex_str);
    terminal_writestring("New allocation at: ");
    terminal_writestring(hex_str);
    terminal_writestring("\n");
    
    // Cleanup
    free(memory1);
    free(memory2);
    free(memory3);
    
    // Wait for user input to return
    terminal_write_colored("\n\nPress ESC to return to main menu...\n", 
                         VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);
                         
    uint8_t scancode;
    while (1) {
        scancode = keyboard_get_scancode();
        if (scancode == KEY_ESC) {
            break;
        }
        sleep_busy(10);
    }
    
    menu_display(main_menu);
}

// Action for PIT test
static void action_pit_test() {
    terminal_write_colored("===== PIT Testing =====\n\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    
    terminal_writestring("Comparing busy-wait (HIGH CPU) vs interrupt (LOW CPU) sleep methods\n");
    terminal_writestring("Each cycle: 1 second busy-wait followed by 1 second interrupt-based\n\n");
    
    uint32_t counter = 0;
    char counter_str[10];
    
    // Only run 5 cycles
    uint8_t max_cycles = 5;
    
    for (uint8_t i = 0; i < max_cycles; i++) {
        // Check for ESC press
        uint8_t scancode = keyboard_get_scancode();
        if (scancode == KEY_ESC) {
            break;
        }
        
        // Update counter
        int_to_str(counter, counter_str);
        
        // Start of test cycle
        terminal_writestring("Cycle [");
        terminal_writestring(counter_str);
        terminal_writestring("]: ");
        
        // Busy wait sleep
        terminal_write_colored("BUSY", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
        sleep_busy(1000);
        terminal_write_colored(" -> ", VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        
        // Interrupt sleep
        terminal_write_colored("INT", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
        sleep_interrupt(1000);
        terminal_writestring(" Complete\n");
        
        // Increment counter
        counter++;
    }
    
    // Wait for user input to return
    terminal_write_colored("\n\nPress ESC to return to main menu...\n", 
                         VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);
                         
    uint8_t scancode;
    while (1) {
        scancode = keyboard_get_scancode();
        if (scancode == KEY_ESC) {
            break;
        }
        sleep_busy(10);
    }
    
    menu_display(main_menu);
}

// Action for music player
static void action_music_player() {
    terminal_write_colored("===== Music Player =====\n\n", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    
    terminal_write_colored("=== PER ARNE SINE SANGÅ ===\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    
    // Create song player
    SongPlayer* player = create_song_player();
    if (player) {
        // Get song structures - using the correct names from song.h
        extern Song music_1;
        extern Song starwars_theme;
        extern Song battlefield_1942_theme;
        extern Song music_2;
        extern Song ode_to_joy;
        extern Song twinkle;
        extern Song music_5;
        extern Song imperial_march;
        
        // Display song list and menu
        terminal_writestring("Song list:\n");
        terminal_writestring("1. Super Mario Theme\n");
        terminal_writestring("2. Battlefield 1942 Theme\n");
        terminal_writestring("3. Tetris Theme\n");
        terminal_writestring("4. Ode to Joy\n");
        terminal_writestring("5. Twinkle Twinkle Little Star\n");
        terminal_writestring("6. Happy Birthday\n");
        terminal_writestring("7. Imperial March (Star Wars)\n");
        terminal_writestring("8. Star Wars Main Theme\n\n");
        
        terminal_write_colored("Select a song (1-8) or press ESC to return to main menu:\n", 
                              VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
        
        // Wait for song selection
        uint8_t scancode;
        bool selected = false;
        
        while (!selected) {
            scancode = keyboard_get_scancode();
            if (scancode == KEY_ESC) {
                terminal_writestring("\nReturning to main menu...\n");
                free(player);
                
                // Wait a moment
                sleep_interrupt(1000);
                menu_display(main_menu);
                return;
            }
            
            // Check for number keys (1-8)
            char c = keyboard_scancode_to_ascii(scancode);
            if (c >= '1' && c <= '8') {
                selected = true;
                
                terminal_writestring("\nPlaying selection: ");
                terminal_putchar(c);
                terminal_writestring("\n(Press ESC after the song to return to menu)\n\n");
                
                // Play the selected song
                switch (c) {
                    case '1':
                        terminal_writestring("Now playing: Super Mario Theme\n");
                        player->play_song(music_1);
                        break;
                    case '2':
                        terminal_writestring("Now playing: Battlefield 1942 Theme\n");
                        player->play_song(battlefield_1942_theme);
                        break;
                    case '3':
                        terminal_writestring("Now playing: Tetris Theme\n");
                        player->play_song(music_2);
                        break;
                    case '4':
                        terminal_writestring("Now playing: Ode to Joy\n");
                        player->play_song(ode_to_joy);
                        break;
                    case '5':
                        terminal_writestring("Now playing: Twinkle Twinkle Little Star\n");
                        player->play_song(twinkle);
                        break;
                    case '6':
                        terminal_writestring("Now playing: Happy Birthday\n");
                        player->play_song(music_5);
                        break;
                    case '7':
                        terminal_writestring("Now playing: Imperial March (Star Wars)\n");
                        player->play_song(imperial_march);
                        break;
                    case '8':
                        terminal_writestring("Now playing: Star Wars Main Theme\n");
                        player->play_song(starwars_theme);
                        break;
                }
                
                terminal_writestring("\nMusic playback complete! Press ESC to return or select another song (1-8):\n");
                selected = false; // Allow selecting another song
            }
            
            sleep_busy(10);
        }
        
        // Clean up
        free(player);
        terminal_write_colored("Music playback complete!\n", VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_BLACK);
    } else {
        terminal_writestring("Failed to create song player.\n");
    }
    
    // Wait for user input to return
    terminal_write_colored("\n\nPress ESC to return to main menu...\n", 
                         VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);
                         
    uint8_t scancode;
    while (1) {
        scancode = keyboard_get_scancode();
        if (scancode == KEY_ESC) {
            break;
        }
        sleep_busy(10);
    }
    
    menu_display(main_menu);
}

// Action for Snake game
static void action_snake_game() {
    snake_game_start();
    menu_display(main_menu);
}