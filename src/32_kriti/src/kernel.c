// kernel.c

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "kprint.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "keyboard.h"
#include "memory.h"
#include "pit.h"
#include "musicplayer.h"
#include "menu.h"  // Add the menu header


#define REST 0
#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 784

#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494

// The linker script defines this symbol (end of kernel).
extern unsigned long end;

// Define a better test song with a wider range of notes
static Note whole_new_world[] = {
    // "I can show you the world"
    {392, 400}, {440, 400}, {494, 400}, {523, 600},
    {0, 200},  // pause

    {494, 400}, {440, 400}, {392, 600},
    {0, 300},  // litt lengre pause

    // "Shining, shimmering, splendid"
    {392, 300}, {440, 300}, {494, 300}, {523, 500},
    {0, 200},

    {494, 300}, {440, 300}, {392, 600},
    {0, 400},

    // "Tell me, princess, now when did you last let your heart decide?"
    {330, 300}, {392, 300}, {440, 300}, {494, 300},
    {0, 200},

    {523, 400}, {494, 300}, {440, 300}, {392, 300},
    {0, 300},

    {440, 600}, {523, 800}
};

// Menu theme configuration
static bool use_borders = true;
static bool center_titles = true;

// Function prototypes for menu system
void run_music_player(void);
void run_keyboard_piano(void);
void show_system_info(void);
void run_memory_test(void);
void show_theme_menu(void);
void apply_default_theme(void);
void apply_blue_theme(void);
void apply_green_theme(void);
void apply_red_theme(void);
void apply_classic_theme(void);
void toggle_borders(void);
void toggle_centered_titles(void);

// Global theme pointers for our menus
static MenuTheme* current_theme = NULL;
static MenuTheme* default_theme = NULL;
static MenuTheme* blue_theme = NULL;
static MenuTheme* green_theme = NULL;
static MenuTheme* red_theme = NULL;
static MenuTheme* classic_theme = NULL;

// Global menu definitions
static Menu main_menu;
static Menu theme_menu;
static Menu theme_options_menu;
static Menu system_menu;
static Menu entertainment_menu;

// Function to run the music player
void run_music_player(void) {
    menu_clear_screen();
    kprint("Music Player\n");
    kprint("------------\n\n");
    kprint("Playing 'A Whole New World'\n");
    kprint("Press ESC to return to menu\n\n");
    
    Song songs[] = {
        { whole_new_world, sizeof(whole_new_world) / sizeof(Note) }
    };
    
    SongPlayer* player = create_song_player();
    if (!player) {
        kprint("Failed to create SongPlayer.\n");
        return;
    }
    
    // Play the song
    player->play_song(&songs[0]);
    
    // Wait for a key before returning to menu
    kprint("\nPress any key to return to menu...\n");
    menu_wait_for_key();
    
    free(player);
}

// Function to run the keyboard piano
void run_keyboard_piano(void) {
    piano_keyboard_mode();
}

// Show system information
void show_system_info(void) {
    menu_clear_screen();
    kprint("System Information\n");
    kprint("=================\n\n");
    
    kprint("Memory Layout:\n");
    print_memory_layout();
    
    kprint("\nSystem ticks: ");
    kprint_dec(get_tick_count());
    kprint("\n\n");
    
    kprint("Press any key to return to menu...\n");
    menu_wait_for_key();
}

// Run memory test
void run_memory_test(void) {
    menu_clear_screen();
    kprint("Memory Test\n");
    kprint("===========\n\n");
    
    kprint("Allocating test memory blocks...\n");
    sleep_interrupt(1000);  // Sleep for 1 second
    
    void* block1 = malloc(10000);
    void* block2 = malloc(20000);
    void* block3 = malloc(30000);
    
    kprint("Block 1 at 0x");
    kprint_hex((unsigned long)block1);
    kprint(" (10000 bytes)\n");
    
    kprint("Block 2 at 0x");
    kprint_hex((unsigned long)block2);
    kprint(" (20000 bytes)\n");
    
    kprint("Block 3 at 0x");
    kprint_hex((unsigned long)block3);
    kprint(" (30000 bytes)\n\n");
    
    kprint("Freeing block 2...\n");
    free(block2);
    
    kprint("Memory layout after free:\n");
    print_memory_layout();
    
    kprint("Freeing remaining blocks...\n");
    free(block1);
    free(block3);
    sleep_interrupt(3000);  // Sleep for 1 second
    kprint("\nPress any key to return to menu...\n");
    menu_wait_for_key();
}

// Apply theme functions
void apply_default_theme(void) {
    menu_set_theme(&main_menu, default_theme);
    menu_set_theme(&theme_menu, default_theme);
    menu_set_theme(&theme_options_menu, default_theme);
    menu_set_theme(&system_menu, default_theme);
    menu_set_theme(&entertainment_menu, default_theme);
    kprint("Applied default theme\n");
}

void apply_blue_theme(void) {
    menu_set_theme(&main_menu, blue_theme);
    menu_set_theme(&theme_menu, blue_theme);
    menu_set_theme(&theme_options_menu, blue_theme);
    menu_set_theme(&system_menu, blue_theme);
    menu_set_theme(&entertainment_menu, blue_theme);
    kprint("Applied blue theme\n");
}

void apply_green_theme(void) {
    menu_set_theme(&main_menu, green_theme);
    menu_set_theme(&theme_menu, green_theme);
    menu_set_theme(&theme_options_menu, green_theme);
    menu_set_theme(&system_menu, green_theme);
    menu_set_theme(&entertainment_menu, green_theme);
    kprint("Applied green theme\n");
}

void apply_red_theme(void) {
    menu_set_theme(&main_menu, red_theme);
    menu_set_theme(&theme_menu, red_theme);
    menu_set_theme(&theme_options_menu, red_theme);
    menu_set_theme(&system_menu, red_theme);
    menu_set_theme(&entertainment_menu, red_theme);
    kprint("Applied red theme\n");
}

void apply_classic_theme(void) {
    menu_set_theme(&main_menu, classic_theme);
    menu_set_theme(&theme_menu, classic_theme);
    menu_set_theme(&theme_options_menu, classic_theme);
    menu_set_theme(&system_menu, classic_theme);
    menu_set_theme(&entertainment_menu, classic_theme);
    kprint("Applied classic theme\n");
}

// Toggle menu border display
void toggle_borders(void) {
    use_borders = !use_borders;
    
    // Update border setting in all themes
    default_theme->draw_border = use_borders;
    blue_theme->draw_border = use_borders;
    green_theme->draw_border = use_borders;
    red_theme->draw_border = use_borders;
    classic_theme->draw_border = use_borders;
    
    kprint("Menu borders: ");
    kprint(use_borders ? "ON" : "OFF");
    kprint("\n");
}

// Toggle centered titles in menus
void toggle_centered_titles(void) {
    center_titles = !center_titles;
    
    // Update title centering in all themes
    default_theme->center_title = center_titles;
    blue_theme->center_title = center_titles;
    green_theme->center_title = center_titles;
    red_theme->center_title = center_titles;
    classic_theme->center_title = center_titles;
    
    kprint("Centered titles: ");
    kprint(center_titles ? "ON" : "OFF");
    kprint("\n");
}

// Initialize all menus
void init_menus(void) {
    // Create themes
    default_theme = menu_create_default_theme();
    blue_theme = menu_create_blue_theme();
    green_theme = menu_create_green_theme();
    red_theme = menu_create_red_theme();
    classic_theme = menu_create_classic_theme();
    
    // Set current theme
    current_theme = default_theme;
    
    // Initialize main menu
    menu_init(&main_menu, "Kriti The Oter");
    menu_set_theme(&main_menu, current_theme);
    
    // Add main menu items
    menu_add_submenu_item(&main_menu, "Entertainment", &entertainment_menu);
    menu_add_submenu_item(&main_menu, "System", &system_menu);
    menu_add_submenu_item(&main_menu, "Appearance", &theme_menu);
    menu_add_action_item(&main_menu, "Shutdown", NULL);  // Action will be handled in main loop
    
    // Initialize entertainment submenu
    menu_init(&entertainment_menu, "Entertainment");
    menu_set_theme(&entertainment_menu, current_theme);
    menu_add_action_item(&entertainment_menu, "Music Player", run_music_player);
    menu_add_action_item(&entertainment_menu, "Keyboard Piano", run_keyboard_piano);
    menu_add_back_item(&entertainment_menu, "Back to Main Menu");
    
    // Initialize system submenu
    menu_init(&system_menu, "System");
    menu_set_theme(&system_menu, current_theme);
    menu_add_action_item(&system_menu, "System Information", show_system_info);
    menu_add_action_item(&system_menu, "Memory Test", run_memory_test);
    menu_add_back_item(&system_menu, "Back to Main Menu");
    
    // Initialize theme menu
    menu_init(&theme_menu, "Appearance");
    menu_set_theme(&theme_menu, current_theme);
    menu_add_action_item(&theme_menu, "Default Theme", apply_default_theme);
    menu_add_action_item(&theme_menu, "Blue Theme", apply_blue_theme);
    menu_add_action_item(&theme_menu, "Green Theme", apply_green_theme);
    menu_add_action_item(&theme_menu, "Red Theme", apply_red_theme);
    menu_add_action_item(&theme_menu, "Classic Theme", apply_classic_theme);
    menu_add_submenu_item(&theme_menu, "Theme Options", &theme_options_menu);
    menu_add_back_item(&theme_menu, "Back to Main Menu");
    
    // Initialize theme options submenu
    menu_init(&theme_options_menu, "Theme Options");
    menu_set_theme(&theme_options_menu, current_theme);
    menu_add_toggle_item(&theme_options_menu, "Show Borders", &use_borders, "ON", "OFF");
    menu_add_toggle_item(&theme_options_menu, "Center Titles", &center_titles, "ON", "OFF");
    menu_add_back_item(&theme_options_menu, "Back to Themes");
    
    kprint("All menus initialized\n");
}

int main(unsigned long magic, struct multiboot_info* mb_info_addr) {
    // Write "Hello World" directly to video memory (VGA text mode).
    const char *str = "Hello World";
    char *video_memory = (char*)0xb8000;
    for (int i = 0; str[i] != '\0'; i++) {
        video_memory[i * 2]     = str[i];
        video_memory[i * 2 + 1] = 0x07;  // White text on black background.
    }

    //kprint("Loading GDT...\n");
    init_gdt();
    //kprint("GDT loaded\n");

    //kprint("Initializing IDT...\n");
    idt_init();
    //kprint("IDT initialized\n");

    //kprint("Initializing ISR...\n");
    isr_init();
    //kprint("ISR initialized\n");

    //kprint("Initializing PIC...\n");
    pic_init();
    //kprint("PIC initialized\n");

    // Enable interrupts.
    //kprint("Enabling interrupts...\n");
    __asm__ volatile ("sti");
    //kprint("Interrupts enabled\n");

    // Initialize kernel memory manager using the address from the linker.
    //kprint("Initializing kernel memory manager...\n");
    init_kernel_memory(&end);

    // Initialize paging.
    //kprint("Initializing paging...\n");
    init_paging();

    // Print current memory layout.
    //kprint("Printing memory layout...\n");
    print_memory_layout();

    // Initialize the PIT.
    //kprint("Initializing PIT...\n");
    init_pit();

    // Display the initial tick count.
    //kprint("Initial tick count: ");
    kprint_dec(get_tick_count());
    kprint("\n");

    // Initialize keyboard input.
    //kprint("Initializing keyboard...\n");
    keyboard_init();

    // Test memory allocation.
    //kprint("\nTesting memory allocation...\n");
    void* some_memory = malloc(12345);
    void* memory2    = malloc(54321);
    void* memory3    = malloc(13331);
    
    //kprint("Allocated memory at: 0x");
    kprint_hex((unsigned long)some_memory);
    kprint("\n");
    
    //kprint("Allocated memory at: 0x");
    kprint_hex((unsigned long)memory2);
    kprint("\n");
    
    //kprint("Allocated memory at: 0x");
    kprint_hex((unsigned long)memory3);
    kprint("\n");

    // Print updated memory layout.
    //kprint("\nUpdated memory layout after allocations:\n");
    print_memory_layout();
    
    // Free one allocation and show updated memory layout.
    //kprint("\nFreeing memory...\n");
    free(memory2);
    //kprint("Memory layout after free:\n");
    print_memory_layout();

    //kprint("\nSystem initialized successfully!\n");
    kprint("System initialized!\n");
    
    // Unmask keyboard IRQ (IRQ1)
    outb(PIC1_DATA_PORT, inb(PIC1_DATA_PORT) & ~0x02);
    void show_boot_art(void) {
        // Clear screen first
        menu_clear_screen();
        
        // Modified Kriti The Oter logo - fits within 80x25 VGA text mode
        kprint("  _  _______  _____ _______ _____    ____ _______ _______ ______ _____  \n");
        kprint(" | |/ /  __ \\|_   _|__   __|_   _|  / __ \\__   __|__   __|  ____|  __ \\ \n");
        kprint(" | ' /| |__) | | |    | |    | |   | |  | | | |     | |  | |__  | |__) |\n");
        kprint(" |  < |  _  /  | |    | |    | |   | |  | | | |     | |  |  __| |  _  / \n");
        kprint(" | . \\| | \\ \\ _| |_   | |   _| |_  | |__| | | |     | |  | |____| | \\ \\ \n");
        kprint(" |_|\\_\\_|  \\_\\_____|  |_|  |_____|  \\____/  |_|     |_|  |______|_|  \\_\\\n");
        kprint("                                                                        \n");
        kprint("                                                                        \n");
        sleep_interrupt(3000);
        
        // Modified otter ASCII art - fits within 80x25 VGA text mode
        kprint("                   ,,,,.                              \n");
        kprint("          ,,,,,,,,,,,,,,,,,                          \n");
        kprint("         ,,,,   ,,,         ,,,,,,                   \n");
        kprint("          ,,,  ,,              ,,,,                  \n");
        kprint("          ,,,                   ,,                   \n");
        kprint("          ,,,  ,,              ,                     \n");
        kprint("    ,,,,,,,,   ,,,    ,,,,     ,,       ,,,,,,,,     \n");
        kprint("         ,,     ,,,,,          ,,,  ,,,,,     ,,,,   \n");
        kprint("    ,,,,,,,     ,,,,,           ,,,,,,          ,,,  \n");
        kprint("          ,,   ,,,,,,,                            ,, \n");
        kprint("           ,,                                      ,,\n");
        kprint("           ,,                      ,,              ,,\n");
        kprint("           ,,                     ,,                ,\n");
        kprint("           ,,        ,,   ,,      ,,                ,,,\n");
        kprint("            ,,      ,,    ,,      ,,                  ,,,,\n");
        kprint("            ,,,     ,,   ,,       ,,,    ,,              ,,,\n");
        kprint("             ,,,,   ,,   ,,      ,,,,    ,,,,,      ,,,,,,,\n");
        kprint("           ,,   ,, ,,, ,,,, ,,,,,,      ,,,,  ,,,,,,,     \n");
        kprint("           ,,,,,,, ,,,,,,,,       ,,,,,,,,,               \n");
        
        // Operating system loading message
        kprint("\n\nUIA Operating System Loading...\n");
        
        // Short pause
        sleep_interrupt(3000); // Just 800 ms delay
    }



    
    // Show boot art
    show_boot_art();
    
    // Initialize menu system
    init_menus();
    
    kprint("Menu system initialized!\n");

    // Main menu system
    while (1) {
        // Run the main menu
        menu_run(&main_menu);

        // Get the selected item
        int selected = menu_get_selected(&main_menu);
        kprint("Selected item: ");
        kprint_dec(selected);
        kprint("\n");    
      
        // Process the selected item
        if (selected == 3) {  // Shutdown option
            menu_clear_screen();
            kprint("\n\n   Shutting down system...\n");
            
            // Halt the CPU
            for (;;) {
                __asm__ volatile ("hlt");
            }
        }
        
        // If we got here, continue looping to show the main menu again
    }
    
    return 0;
}