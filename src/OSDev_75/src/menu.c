#include "menu.h"
#include "pong.h"
#include "music_player.h"
#include "text_editor.h"
#include "arch/i386/GDT/util.h" 
#include <string.h> 
#include "libc/stddef.h"

MenuState current_state = MENU_STATE_MAIN;
uint8_t selected_option = 0;
uint8_t last_scancode = 0;

const char* main_menu_options[] = {
    "1. Play Pong",
    "2. Music Player",
    "3. Text editor",
    "4. About",
    "5. Exit"
};

#define NUM_MAIN_MENU_OPTIONS 5

#define RAIN_DURATION 10000
#define MAX_DROPS 60
#define MAX_TRAIL 15

static uint32_t next_random = 1;

uint32_t os_rand() {
    next_random = next_random * 1103515245 + 12345;
    return (next_random >> 16) & 0x7FFF;
}

void os_srand(uint32_t seed) {
    next_random = seed;
}

uint32_t os_rand_range(uint32_t max) {
    return os_rand() % max;
}

const char matrix_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{}|;:,.<>/?";
const int matrix_chars_len = sizeof(matrix_chars) - 1;

typedef struct {
    int x;
    int y;
    int speed;
    int trail_length;
    char chars[MAX_TRAIL];
    uint8_t colors[MAX_TRAIL];
} Raindrop;

void init_menu() {
    current_state = MENU_STATE_MAIN;
    selected_option = 0;
}

void fade_transition(uint8_t start_color, uint8_t end_color, uint16_t duration_ms) {
    uint16_t steps = 8;
    uint16_t delay_per_step = duration_ms / steps;
    
    for (uint16_t step = 0; step <= steps; step++) {
        uint8_t color = (step == steps) ? end_color : start_color;
        
        Reset();
        setColor(UIA_WHITE, color);
        
        for (uint16_t y = 0; y < getScreenHeight(); y++) {
            for (uint16_t x = 0; x < getScreenWidth(); x++) {
                putCharAt(x, y, ' ', UIA_WHITE, color);
            }
        }
        
        sleep_interrupt(delay_per_step);
    }
}

void wipe_screen_transition() {
    fade_transition(COLOR8_BLACK, UIA_RED, 500);
    fade_transition(UIA_RED, COLOR8_BLACK, 500);
}

void init_raindrop(Raindrop* drop, int screen_width) {
    drop->x = os_rand_range(screen_width);
    drop->y = -(int)(os_rand_range(10));
    drop->speed = 1 + os_rand_range(3);
    drop->trail_length = 5 + os_rand_range(MAX_TRAIL - 5);
    
    for (int i = 0; i < drop->trail_length; i++) {
        drop->chars[i] = matrix_chars[os_rand_range(matrix_chars_len)];
        
        if (i == 0) {
            drop->colors[i] = COLOR8_WHITE;
        } else if (i < drop->trail_length / 3) {
            drop->colors[i] = COLOR8_LIGHT_GREEN;
        } else {
            drop->colors[i] = COLOR8_GREEN;
        }
    }
}

void update_raindrop(Raindrop* drop, int screen_height, int screen_width) {
    drop->y += drop->speed;
    
    if (os_rand_range(3) == 0) {
        int pos = os_rand_range(drop->trail_length);
        drop->chars[pos] = matrix_chars[os_rand_range(matrix_chars_len)];
    }
    
    if (drop->y - drop->trail_length > screen_height) {
        init_raindrop(drop, screen_width);
    }
}

void draw_raindrop(Raindrop* drop, int screen_height) {
    for (int i = 0; i < drop->trail_length; i++) {
        int y = drop->y - i;
        
        if (y >= 0 && y < screen_height) {
            putCharAt(drop->x, y, drop->chars[i], drop->colors[i], COLOR8_BLACK);
        }
    }
}

void draw_pulsing_uia_logo(int frame, bool final_frame) {
    uint16_t screen_width = getScreenWidth();
    uint16_t screen_height = getScreenHeight();
    uint16_t center_x = screen_width / 2 - 10;
    uint16_t center_y = screen_height / 2 - 4;
    
    uint8_t bg_color;
    
    if (final_frame) {
        bg_color = UIA_RED;
    } else {
        int pulse = frame % 10;
        
        if (pulse < 5) {
            bg_color = COLOR8_RED;
        } else {
            bg_color = UIA_RED;
        }
    }
    
    for (uint16_t y = center_y - 2; y < center_y + 8; y++) {
        for (uint16_t x = center_x - 2; x < center_x + 22; x++) {
            if (x < screen_width && y < screen_height) {
                putCharAt(x, y, ' ', COLOR8_WHITE, bg_color);
            }
        }
    }
    
    setColor(COLOR8_WHITE, bg_color);
    
    setCursorPosition(center_x, center_y);
    print(" _   _ _    _    ");
    setCursorPosition(center_x, center_y + 1);
    print("| | | (_)  / \\   ");
    setCursorPosition(center_x, center_y + 2);
    print("| | | |_  / _ \\  ");
    setCursorPosition(center_x, center_y + 3);
    print("| |_| | |/ ___ \\ ");
    setCursorPosition(center_x, center_y + 4);
    print(" \\___/|_/_/   \\_\\");
    
    if (final_frame) {
        setCursorPosition(center_x, center_y + 6);
        print("University of Agder");
    }
}

const char* get_random_uia_word() {
    const char* words[] = {
        "UiA", "Agder", "University", "Norway", "OS", "Computer", 
        "Science", "Technology", "Innovation", "Research", "Education",
        "Knowledge", "Future", "Development", "Programming", "System",
        "Network", "Digital", "Data", "Algorithm", "Software"
    };
    const int num_words = sizeof(words) / sizeof(words[0]);
    
    return words[os_rand_range(num_words)];
}

void matrix_rain_effect() {
    uint16_t screen_width = getScreenWidth();
    uint16_t screen_height = getScreenHeight();
    
    os_srand(get_current_tick());
    
    Raindrop raindrops[MAX_DROPS];
    for (int i = 0; i < MAX_DROPS; i++) {
        init_raindrop(&raindrops[i], screen_width);
    }
    
    Reset();
    for (uint16_t y = 0; y < screen_height; y++) {
        for (uint16_t x = 0; x < screen_width; x++) {
            putCharAt(x, y, ' ', COLOR8_GREEN, COLOR8_BLACK);
        }
    }
    
    int word_timer = 0;
    const int WORD_INTERVAL = 15;
    int word_x = 0, word_y = 0;
    const char* current_word = 0;
    
    uint32_t start_time = get_current_tick();
    uint32_t elapsed_time;
    int frame = 0;
    
    do {
        elapsed_time = (get_current_tick() - start_time) * 10;
        
        for (uint16_t y = 0; y < screen_height; y++) {
            for (uint16_t x = 0; x < screen_width; x++) {
                if (os_rand_range(10) == 0) {
                    putCharAt(x, y, ' ', COLOR8_GREEN, COLOR8_BLACK);
                }
            }
        }
        
        for (int i = 0; i < MAX_DROPS; i++) {
            update_raindrop(&raindrops[i], screen_height, screen_width);
            draw_raindrop(&raindrops[i], screen_height);
        }
        
        if (++word_timer >= WORD_INTERVAL) {
            word_timer = 0;
            word_x = os_rand_range(screen_width - 15);
            word_y = os_rand_range(screen_height);
            current_word = get_random_uia_word();
        }
        
        if (current_word != 0) {
            setColor(COLOR8_LIGHT_GREEN, COLOR8_BLACK);
            setCursorPosition(word_x, word_y);
            print(current_word);
        }
        
        if (elapsed_time > RAIN_DURATION - 1000) {
            int fade_progress = (elapsed_time - (RAIN_DURATION - 1000)) * 100 / 1000;
            
            if (frame % 2 == 0) {
                draw_pulsing_uia_logo(frame, false);
            }
        }
        
        sleep_interrupt(50);
        frame++;
        
    } while (elapsed_time < RAIN_DURATION);
    
    draw_pulsing_uia_logo(frame, true);
    
    sleep_interrupt(1000);
}

void enhanced_uia_splash() {
    matrix_rain_effect();
    fade_transition(UIA_RED, COLOR8_BLACK, 1000);
}

void show_uia_splash() {
    Reset();
    
    for (uint16_t y = 0; y < getScreenHeight(); y++) {
        for (uint16_t x = 0; x < getScreenWidth(); x++) {
            putCharAt(x, y, ' ', UIA_WHITE, UIA_RED);
        }
    }
    
    uint16_t center_x = getScreenWidth() / 2 - 10;
    uint16_t center_y = getScreenHeight() / 2 - 4;
    
    setColor(UIA_WHITE, UIA_RED);
    
    setCursorPosition(center_x, center_y);
    print(" _   _ _    _    ");
    setCursorPosition(center_x, center_y + 1);
    print("| | | (_)  / \\   ");
    setCursorPosition(center_x, center_y + 2);
    print("| | | |_  / _ \\  ");
    setCursorPosition(center_x, center_y + 3);
    print("| |_| | |/ ___ \\ ");
    setCursorPosition(center_x, center_y + 4);
    print(" \\___/|_/_/   \\_\\");
    
    setCursorPosition(center_x, center_y + 6);
    print("University of Agder");
    
    sleep_interrupt(2000);
    
    fade_transition(UIA_RED, COLOR8_BLACK, 1000);
}

void draw_menu_border(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    for (uint16_t i = x + 1; i < x + w - 1; i++) {
        putCharAt(i, y, 205, MENU_BORDER_COLOR, MENU_BG_COLOR);
        putCharAt(i, y + h - 1, 205, MENU_BORDER_COLOR, MENU_BG_COLOR);
    }
    
    for (uint16_t i = y + 1; i < y + h - 1; i++) {
        putCharAt(x, i, 186, MENU_BORDER_COLOR, MENU_BG_COLOR);
        putCharAt(x + w - 1, i, 186, MENU_BORDER_COLOR, MENU_BG_COLOR);
    }
    
    putCharAt(x, y, 201, MENU_BORDER_COLOR, MENU_BG_COLOR);
    putCharAt(x + w - 1, y, 187, MENU_BORDER_COLOR, MENU_BG_COLOR);
    putCharAt(x, y + h - 1, 200, MENU_BORDER_COLOR, MENU_BG_COLOR);
    putCharAt(x + w - 1, y + h - 1, 188, MENU_BORDER_COLOR, MENU_BG_COLOR);
}

void draw_menu_options(const char** options, uint8_t count, uint8_t selected) {
    for (uint8_t i = 0; i < count; i++) {
        setCursorPosition(MENU_START_X + 5, MENU_START_Y + 2 + i * 2);
        
        if (i == selected) {
            setColor(MENU_SELECTED_COLOR, MENU_SELECTED_BG);
            
            for (uint16_t j = 0; j < MENU_WIDTH - 10; j++) {
                putCharAt(MENU_START_X + 3 + j, MENU_START_Y + 2 + i * 2, ' ', 
                          MENU_SELECTED_COLOR, MENU_SELECTED_BG);
            }
            
            setCursorPosition(MENU_START_X + 5, MENU_START_Y + 2 + i * 2);
            print("> ");
            print(options[i]);
        } else {
            setColor(MENU_TEXT_COLOR, MENU_BG_COLOR);
            print("  ");
            print(options[i]);
        }
    }
}

void draw_title_bar(const char* title) {
    for (uint16_t i = MENU_START_X; i < MENU_START_X + MENU_WIDTH; i++) {
        putCharAt(i, MENU_START_Y - 2, ' ', UIA_WHITE, UIA_RED);
    }
    
    uint16_t title_len = 0;
    for (const char* p = title; *p; p++) {
        title_len++;
    }
    uint16_t title_pos = MENU_START_X + (MENU_WIDTH - title_len) / 2;
    setCursorPosition(title_pos, MENU_START_Y - 2);
    setColor(UIA_WHITE, UIA_RED);
    print(title);
}

void draw_footer(const char* message) {
    setCursorPosition(MENU_START_X + 2, MENU_START_Y + MENU_HEIGHT);
    setColor(MENU_TEXT_COLOR, MENU_BG_COLOR);
    print(message);
}

void draw_menu_shadow() {
    uint8_t shadow_color = COLOR8_DARK_GREY; 
    
    for (uint16_t i = 0; i < MENU_HEIGHT; i++) {
        putCharAt(MENU_START_X + MENU_WIDTH, MENU_START_Y + i, ' ', 
                  shadow_color, shadow_color);
        putCharAt(MENU_START_X + MENU_WIDTH + 1, MENU_START_Y + i + 1, ' ', 
                  shadow_color, shadow_color);
    }
    
    for (uint16_t i = 0; i < MENU_WIDTH + 1; i++) {
        putCharAt(MENU_START_X + i, MENU_START_Y + MENU_HEIGHT, ' ', 
                  shadow_color, shadow_color);
        putCharAt(MENU_START_X + i + 1, MENU_START_Y + MENU_HEIGHT + 1, ' ', 
                  shadow_color, shadow_color);
    }
}

void animate_menu_open() {
    uint16_t start_width = 10;
    uint16_t start_height = 5;
    
    for (uint16_t w = start_width; w <= MENU_WIDTH; w += 1) { 
        uint16_t h = start_height + (w - start_width) * (MENU_HEIGHT - start_height) / (MENU_WIDTH - start_width);
        uint16_t x = MENU_START_X + (MENU_WIDTH - w) / 2;
        uint16_t y = MENU_START_Y + (MENU_HEIGHT - h) / 2;
        
        for (uint16_t i = MENU_START_X; i < MENU_START_X + MENU_WIDTH; i++) {
            for (uint16_t j = MENU_START_Y; j < MENU_START_Y + MENU_HEIGHT; j++) {
                putCharAt(i, j, ' ', MENU_TEXT_COLOR, MENU_BG_COLOR);
            }
        }
        
        draw_menu_border(x, y, w, h);
        
        sleep_interrupt(10); 
    }
}

void draw_system_info() {
    char buffer[30];
    uint16_t screen_width = getScreenWidth();
    
    uint32_t total_mem = 640; 
    uint32_t free_mem = 512;  
    
    setCursorPosition(screen_width - 25, 1);
    setColor(UIA_WHITE, UIA_RED); 
    
    buffer[0] = 'M';
    buffer[1] = 'e';
    buffer[2] = 'm';
    buffer[3] = ':';
    buffer[4] = ' ';
    
    uint32_t used_mem = total_mem - free_mem;
    uint8_t pos = 5;
    
    uint32_t temp = used_mem;
    uint8_t digits = 0;
    do {
        digits++;
        temp /= 10;
    } while (temp > 0);
    
    temp = used_mem;
    pos += digits;
    uint8_t end_pos = pos;
    
    do {
        buffer[--pos] = '0' + (temp % 10);
        temp /= 10;
    } while (temp > 0);
    
    pos = end_pos;
    buffer[pos++] = 'K';
    buffer[pos++] = '/';
    
    temp = total_mem;
    digits = 0;
    do {
        digits++;
        temp /= 10;
    } while (temp > 0);
    
    temp = total_mem;
    pos += digits;
    end_pos = pos;
    
    do {
        buffer[--pos] = '0' + (temp % 10);
        temp /= 10;
    } while (temp > 0);
    
    pos = end_pos;
    buffer[pos++] = 'K';
    buffer[pos] = '\0';
    
    print(buffer);
    
    setCursorPosition(screen_width - 10, 1);
    uint32_t ticks = get_current_tick() / 100; 
    
    uint8_t minutes = (ticks / 60) % 60;
    uint8_t seconds = ticks % 60;
    
    buffer[0] = '0' + (minutes / 10);
    buffer[1] = '0' + (minutes % 10);
    buffer[2] = ':';
    buffer[3] = '0' + (seconds / 10);
    buffer[4] = '0' + (seconds % 10);
    buffer[5] = '\0';
    
    print(buffer);
}

void draw_main_menu() {
    Reset();
    
    draw_menu_shadow();
    
    draw_title_bar("UiA OS - Assignment 6 - Improvisation");
    
    draw_menu_border(MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT);
    
    draw_menu_options(main_menu_options, NUM_MAIN_MENU_OPTIONS, selected_option);
    
    draw_system_info();
    
    draw_footer("Use UP/DOWN arrows to navigate, ENTER to select, ESC to exit");
}

void handle_menu_input() {
    uint8_t scancode = inPortB(0x60);
    bool keyReleased = scancode & 0x80;
    scancode &= 0x7F;
    
    if (!keyReleased) {
        last_scancode = scancode;
        
        switch (current_state) {
            case MENU_STATE_MAIN:
                switch (scancode) {
                    case 0x48:
                        if (selected_option > 0) {
                            selected_option--;
                        }
                        break;
                    case 0x50:
                        if (selected_option < NUM_MAIN_MENU_OPTIONS - 1) {
                            selected_option++;
                        }
                        break;
                    case 0x1C:
                        switch (selected_option) {
                            case 0:
                                wipe_screen_transition();
                                current_state = MENU_STATE_PONG;
                                init_pong();
                                break;
                            case 1:
                                wipe_screen_transition();
                                current_state = MENU_STATE_MUSIC_PLAYER;
                                init_music_player();
                                break;
                            case 2:
                                wipe_screen_transition();
                                current_state = MENU_STATE_TEXT_EDITOR;
                                init_text_editor();
                                break;
                            case 3:
                                wipe_screen_transition();
                                current_state = MENU_STATE_ABOUT;
                                show_about();
                                break;
                            case 4:
                                wipe_screen_transition();
                                Reset();
                                setColor(UIA_WHITE, COLOR8_BLACK);
                                print("Returned to OS prompt. Press any key to restart menu.\n");
                                break;
                        }
                        break;
                }
                break;
            default:
                if (scancode == 0x01) {
                    wipe_screen_transition();
                    current_state = MENU_STATE_MAIN;
                }
                break;
        }
    }
}

void menu_loop() {
    enhanced_uia_splash();
    
    animate_menu_open();
    
    while (1) {
        switch (current_state) {
            case MENU_STATE_MAIN:
                draw_main_menu();
                break;
            case MENU_STATE_PONG:
                pong_loop();
                break;
            case MENU_STATE_MUSIC_PLAYER: 
                music_player_loop();
                break;
            case MENU_STATE_TEXT_EDITOR:
                text_editor_loop();
                break;
            case MENU_STATE_ABOUT:
                break;
            default:
                current_state = MENU_STATE_MAIN;
                break;
        }
        
        handle_menu_input();
        
        sleep_interrupt(50);
    }
}

void run_menu() {
    init_menu();
    menu_loop();
}

void show_about() {
    Reset();
    
    draw_menu_shadow();
    
    draw_title_bar("About UiA OS");
    
    draw_menu_border(MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT);
    
    for (uint16_t y = MENU_START_Y + 1; y < MENU_START_Y + MENU_HEIGHT - 1; y++) {
        for (uint16_t x = MENU_START_X + 1; x < MENU_START_X + MENU_WIDTH - 1; x++) {
            putCharAt(x, y, ' ', MENU_TEXT_COLOR, MENU_BG_COLOR);
        }
    }
    
    setColor(UIA_WHITE, UIA_RED);
    setCursorPosition(MENU_START_X + 12, MENU_START_Y + 2);
    print("About UiA OS Improvisation");
    
    setColor(MENU_TEXT_COLOR, MENU_BG_COLOR);
    setCursorPosition(MENU_START_X + 2, MENU_START_Y + 4);
    print("This OSDev_75 for the Operating Systems course.");
    
    setCursorPosition(MENU_START_X + 2, MENU_START_Y + 5);
    print("University of Agder - 2025");
    
    setColor(UIA_RED, MENU_BG_COLOR);
    setCursorPosition(MENU_START_X + 2, MENU_START_Y + 7);
    print("Features:");
    
    setColor(MENU_TEXT_COLOR, MENU_BG_COLOR);
    setCursorPosition(MENU_START_X + 4, MENU_START_Y + 8);
    print("- Pong Game with keyboard controls");
    
    setCursorPosition(MENU_START_X + 4, MENU_START_Y + 9);
    print("- Text Editor for simple note-takin");
    
    setCursorPosition(MENU_START_X + 4, MENU_START_Y + 10);
    print("- UiA-branded user interface");
    
    setColor(UIA_RED, MENU_BG_COLOR);
    setCursorPosition(MENU_START_X + 2, MENU_START_Y + MENU_HEIGHT - 3);
    print("Press ESC to return to the main menu");
}

extern void init_music_player();
extern void music_player_loop();

extern void init_text_editor();
extern void text_editor_loop();