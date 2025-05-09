#include "loading_screen.h"
#include <libc/stddef.h> // Add this for NULL definition

// ASCII art logo for Daemon Duo OS - custom text version
static const char* daemon_duo_logo[] = {
    " ____                                  ____              ",
    "|  _ \\  __ _  ___ _ __ ___   ___  _ _|  _ \\ _   _  ___  ",
    "| | | |/ _` |/ _ \\ '_ ` _ \\ / _ \\| | | | | | | | |/ _ \\ ",
    "| |_| | (_| |  __/ | | | | | (_) | | | |_| | |_| | (_) |",
    "|____/ \\__,_|\\___|_| |_| |_|\\___/|_|_|____/ \\__,_|\\___/ ",
    NULL
};

// Progress bar frames
static const char* progress_frames[] = {
    "[          ]",
    "[=         ]",
    "[==        ]",
    "[===       ]",
    "[====      ]",
    "[=====     ]",
    "[======    ]",
    "[=======   ]",
    "[========  ]",
    "[========= ]",
    "[==========]"
};

// Calculate the length of a string
size_t terminal_strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Set cursor position explicitly
void terminal_set_position(int row, int col) {
    terminal_row = row;
    terminal_column = col;
    update_cursor(row, col);
}

// Print a centered string at a specific row
void print_centered(const char* str, int row) {
    size_t len = terminal_strlen(str);
    int col = (TERMINAL_WIDTH - len) / 2;
    if (col < 0) col = 0;
    
    terminal_set_position(row, col);
    writeline(str);
}

// Display the loading screen with animation
void display_loading_screen(void) {
    // Clear the terminal
    terminal_clear();
    
    // Calculate vertical centering for the logo
    int logo_height = 0;
    while (daemon_duo_logo[logo_height] != NULL) {
        logo_height++;
    }
    
    int start_row = (TERMINAL_HEIGHT - logo_height - 5) / 2;
    if (start_row < 0) start_row = 0;
    
    // Display the logo
    for (int i = 0; daemon_duo_logo[i] != NULL; i++) {
        print_centered(daemon_duo_logo[i], start_row + i);
    }
    
    // Display welcome message
    print_centered("Welcome to Daemon Duo OS", start_row + logo_height + 1);
    
    // Animated loading bar
    int progress_row = start_row + logo_height + 3;
    
    // Display each frame of the loading animation
    // Fix signedness comparison warning by casting size to int
    for (int i = 0; i < (int)(sizeof(progress_frames) / sizeof(progress_frames[0])); i++) {
        print_centered(progress_frames[i], progress_row);
        
        // Create and display status message
        char status_prefix[] = "Loading system... ";
        char status_suffix[] = "%";
        
        // Calculate position for centering the status message
        int percent = i * 10;
        int digits = 1;
        if (percent >= 10) digits = 2;
        if (percent >= 100) digits = 3;
        
        int total_len = terminal_strlen(status_prefix) + digits + terminal_strlen(status_suffix);
        int col = (TERMINAL_WIDTH - total_len) / 2;
        if (col < 0) col = 0;
        
        // Position cursor and print status message in parts
        terminal_set_position(progress_row + 1, col);
        writeline(status_prefix);
        
        // Print percentage using existing printf function
        printf("%d", percent);
        writeline(status_suffix);
        
        // Wait before next frame
        sleep_interrupt(200);
    }
    
    // Show boot complete message
    print_centered("System loaded successfully!", progress_row + 1);
    sleep_interrupt(1000);
}
