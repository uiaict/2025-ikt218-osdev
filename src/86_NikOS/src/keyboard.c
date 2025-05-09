#include "ports.h"
#include "terminal.h"
#include "libc/stdint.h"
#include "libc/string.h"
#include "keyboard.h"
#include "pit.h"
#include "rng.h"
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "song.h"
#include "snake.h"
#include "welcome.h"

int32_t shift_pressed = 0;
static bool extended_scancode = false;

#define CMD_BUFFER_SIZE 512
#define MAX_ARGS 32
char cmd_buffer[CMD_BUFFER_SIZE];
int32_t cmd_index = 0;

#define HISTORY_SIZE 32
char history[HISTORY_SIZE][CMD_BUFFER_SIZE];
int32_t history_start = 0;
int32_t history_count = 0;
int32_t history_index = -1;

uint32_t last_key_tick = 0;

#define TAB_COMPLETION_COLOR VGA_COLOR_LIGHT_GREY
#define TAB_COMPLETION_BUFFER_SIZE 32
char tab_completion_buffer[TAB_COMPLETION_BUFFER_SIZE];
const char* tab_completable_commands[] = {
    "help",
    "clear",
    "echo",
    "music",
    "snake",
    "nikos",
    "uptime",
    "roll",
    "rand",
    "randint",
    NULL
};
const char* tab_completable_commands_help[] = {
    "Display help information",
    "Clear the terminal screen",
    "Echo the input string",
    "Play the star wars theme",
    "Play snake",
    "Display the NikOS logo",
    "Display system uptime",
    "Roll dice (e.g., 2d6)",
    "Generate a random number",
    "Generate a random integer between two numbers",
    NULL
};

static bool playing_snake = false;

char scancode_to_ascii[128] = {
    0, 27, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7',
    '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

char shifted_scancode_to_ascii[128] = {
    0, 27, '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{', '}', '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
    '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7',
    '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

static bool parse_roll_command(const char* str, uint32_t* count, uint32_t* sides) {
    *count = 0;
    *sides = 0;

    while (*str && *str != 'd' && *str != 'D') {
        if (*str < '0' || *str > '9') return false;
        *count = *count * 10 + (*str - '0');
        str++;
    }

    if (*str != 'd' && *str != 'D') return false;
    str++;

    while (*str && *str != ' ') {
        if (*str < '0' || *str > '9') return false;
        *sides = *sides * 10 + (*str - '0');
        str++;
    }

    return *str == '\0' && *count > 0 && *sides > 0;
}

static bool parse_randint_command(const char* str_min, const char* str_max, uint32_t* min, uint32_t* max) {
    *min = 0;
    *max = 0;

    while (*str_min) {
        if (*str_min < '0' || *str_min > '9') return false;
        *min = *min * 10 + (*str_min - '0');
        str_min++;
    }

    while (*str_max) {
        if (*str_max < '0' || *str_max > '9') return false;
        *max = *max * 10 + (*str_max - '0');
        str_max++;
    }

    return *str_min == '\0' && *str_max == '\0' && *min < *max;
}

void save_command_to_history(const char* cmd) {
    if (*cmd == '\0') return; // Ignore empty commands

    if (history_count > 0 && strncmp(history[(history_start + history_count - 1) % HISTORY_SIZE], cmd, CMD_BUFFER_SIZE) == 0) {
        return; // Ignore duplicate commands
    }

    int32_t index = (history_start + history_count) % HISTORY_SIZE;
    strncpy(history[index], cmd, CMD_BUFFER_SIZE - 1);
    history[index][CMD_BUFFER_SIZE - 1] = '\0';
    if (history_count < HISTORY_SIZE) {
        history_count++;
    } else {
        history_start = (history_start + 1) % HISTORY_SIZE;
    }
}

void recall_history(int32_t direction, char* input_buffer, int32_t* input_index) {
    if (history_count == 0) return;

    if (direction == -1) {
        if (history_index < history_count - 1) {
            history_index++;
        }
    } else if (direction == 1) {
        if (history_index > 0) {
            history_index--;
        }
    }

    clear_input_line();

    int32_t index = (history_start + history_count - 1 - history_index) % HISTORY_SIZE;
    strncpy(input_buffer, history_index >= 0 ? history[index] : "", CMD_BUFFER_SIZE - 1);
    *input_index = strlen(input_buffer);

    terminal_writestring(input_buffer);
}

void execute_command(const char* cmd) {
    save_command_to_history(cmd);
    history_index = -1;

    char* argv[MAX_ARGS];
    int32_t argc = 0;

    static char buffer[CMD_BUFFER_SIZE];
    strncpy(buffer, cmd, CMD_BUFFER_SIZE);
    buffer[CMD_BUFFER_SIZE - 1] = '\0';

    char* token = strtok(buffer, " ");
    while (token != NULL && argc < MAX_ARGS - 1) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    argv[argc] = NULL;
    if (argc == 0) return;  // No command entered

    if (strncmp(argv[0], "clear", 5) == 0) {
        terminal_clear();
    } else if (strncmp(argv[0], "echo", 4) == 0) {
        for (int32_t i = 1; i < argc; i++) {
            terminal_writestring(argv[i]);
            if (i < argc - 1) terminal_putchar(' ');
        }
        terminal_putchar('\n');
    } else if (strncmp(argv[0], "roll", 4) == 0) {
        uint32_t count;
        uint32_t sides;
        if (parse_roll_command(argv[1], &count, &sides)) {
            terminal_writeuint_color(roll_dice(count, sides), get_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
            terminal_putchar('\n');
        } else {
            terminal_writestring_color("Invalid roll command format. Use <count>d<sides>.\n", get_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
        }
    } else if (strncmp(argv[0], "randint", 7) == 0) {
        uint32_t min;
        uint32_t max;
        if (parse_randint_command(argv[1], argv[2], &min, &max)) {
            terminal_writeuint_color(rand_range(min, max), get_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
            terminal_putchar('\n');
        } else {
            terminal_writestring_color("Invalid RNG command format. Use <min> <max>.\n", get_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
        }
    } else if (strncmp(argv[0], "rand", 3) == 0) {
        terminal_writeuint_color(rand(), get_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
        terminal_putchar('\n');
    } else if (strncmp(argv[0], "music", 5) == 0) {
        terminal_writestring_color("Execute order 66\n", get_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
        play_star_wars();
    } else if (strncmp(argv[0], "snake", 5) == 0) {
        playing_snake = true;
        snake_init();
    } else if (strncmp(argv[0], "nikos", 5) == 0) {
        welcome_message();
    } else if (strncmp(argv[0], "help", 4) == 0) {
        terminal_writestring_color("Available commands:\n", get_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK));
        for (int32_t i = 0; tab_completable_commands[i] != NULL; i++) {
            terminal_writestring_color(tab_completable_commands[i], get_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
            terminal_writestring_color(" - ", get_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
            terminal_writestring_color(tab_completable_commands_help[i], get_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK));
            terminal_putchar('\n');
        }
    } else if (strncmp(argv[0], "uptime", 6) == 0) {;
        terminal_writeuint_color(pit_get_ticks() / TICKS_PER_MS, get_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK));
        terminal_writestring_color("ms\n", get_color(VGA_COLOR_BROWN, VGA_COLOR_BLACK));
    } else {
        terminal_writestring("Unknown command: ");
        terminal_writestring(argv[0]);
        terminal_putchar('\n');
    }

    terminal_writestring("> ");
}

void tab_completion() {
    if (tab_completion_buffer[0] == '\0') return;
    for (int i = 0; tab_completion_buffer[i] != '\0' && cmd_index < CMD_BUFFER_SIZE - 1; i++) {
        char c = tab_completion_buffer[i];
        cmd_buffer[cmd_index++] = c;
        terminal_putchar(c);
    }
    memset(tab_completion_buffer, 0, TAB_COMPLETION_BUFFER_SIZE);
}

void tab_completion_prompt() {
    size_t old_len = strlen(tab_completion_buffer);
    if (old_len > 0) {
        size_t col = terminal_get_column();
        size_t row = terminal_get_row();
        for (size_t i = 0; i < old_len + 1; i++) terminal_putchar(' ');
        terminal_setcursor(col, row);
        memset(tab_completion_buffer, 0, TAB_COMPLETION_BUFFER_SIZE);
    }

    if (cmd_buffer[0] == '\0') return;
    if (strcontains(cmd_buffer, ' ')) return;

    for (int32_t i = 0; tab_completable_commands[i] != NULL; i++) {
        if (strncmp(tab_completable_commands[i], cmd_buffer, strlen(cmd_buffer)) == 0) {
            size_t col = terminal_get_column();
            size_t row = terminal_get_row();
            
            const char* command = tab_completable_commands[i];
            size_t input_len = strlen(cmd_buffer);

            strncpy(tab_completion_buffer, command + input_len, CMD_BUFFER_SIZE - input_len - 1);
            tab_completion_buffer[CMD_BUFFER_SIZE - input_len - 1] = '\0';
            
            terminal_writestring_color(tab_completion_buffer, get_color(TAB_COMPLETION_COLOR, VGA_COLOR_BLACK));
            terminal_setcursor(col, row);

            return;
        }
    }
}

void keyboard_handler() {
    uint8_t scancode = inb(0x60);

    if (playing_snake) {
        snake_change_direction(scancode);
        return;
    }

    if (scancode == 0xE0) {
        extended_scancode = true;
        return;
    }

    if (extended_scancode) {
        switch (scancode) {
            case 0x48:  // Up arrow
                recall_history(-1, cmd_buffer, &cmd_index);
                break;
            case 0x50:  // Down arrow
                recall_history(1, cmd_buffer, &cmd_index);
                break;
            case 0x4B:  // Left arrow
                break;
            case 0x4D:  // Right arrow
                break;
        }
        extended_scancode = false;
        return;
    }

    // RNG BISH
    uint32_t current_tick = pit_get_ticks();
    uint32_t delta = current_tick - last_key_tick;
    last_key_tick = current_tick;

    rng_seed_xor(delta);
    uint32_t _ = rand();

    // Handle key releases
    if (scancode & 0x80) {
        // Shift released
        if (scancode == 0xAA || scancode == 0xB6) shift_pressed = 0;
        return;
    }

    // Shift pressed
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        return;
    }

    char c = shift_pressed ? shifted_scancode_to_ascii[scancode] : scancode_to_ascii[scancode];

    if (!c) return;

    if (c == '\n') {
        terminal_putchar('\n');
        cmd_buffer[cmd_index] = '\0';
        execute_command(cmd_buffer);
        memset(cmd_buffer, 0, CMD_BUFFER_SIZE);
        cmd_index = 0;
        return;
    }

    if (c == '\b') {
        if (cmd_index > 0) {
            cmd_buffer[--cmd_index] = '\0';
            terminal_setcursor(terminal_get_column() - 1, terminal_get_row());
            terminal_putchar(' ');
            terminal_setcursor(terminal_get_column() - 1, terminal_get_row());
        }
        tab_completion_prompt();
        return;
    }

    if (c == '\t') {
        tab_completion();
        return;
    }

    if (cmd_index < CMD_BUFFER_SIZE - 1) {
        cmd_buffer[cmd_index++] = c;
        terminal_putchar(c);
        tab_completion_prompt();
    }
}

void keyboard_install() {
    outb(0x21, inb(0x21) & ~0x02);
}

void not_playing_snake() {
    playing_snake = false;
}

bool is_playing_snake() {
    return playing_snake;
}