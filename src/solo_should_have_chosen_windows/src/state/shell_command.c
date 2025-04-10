#include "state/shell_command.h"
#include "terminal/cursor.h"
#include "terminal/print.h"
#include "memory/heap.h"

#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "libc/stdint.h"

#define SCREEN_WIDTH 80
#define LAST_ROW 24
#define VGA_ADDRESS 0xB8000

// Shell stubs
const char* launch_stub = "shc-launch";
const char* info_stub = "info";
const char* help_stub = "help";
const char* clear_stub = "clear";
const char* music_player_stub = "music";
const char* art_launch_stub = "art";
const char* heap_print_stub = "heap";

// Music specific stubs
const char* music_command_stub = "shc-music";
const char* music_command_play = "play";
const char* music_command_list = "list";
const char* music_command_exit = "exit";
const char* music_command_info = "info";

// Art specific stubs
const char* art_command_stub = "shc-art";
const char* art_command_list = "list";
const char* art_command_exit = "exit";
const char* art_command_new = "new";
const char* art_command_load = "load";
const char* art_command_delete = "delete";

const char* shell_command_not_found = "Command not found.\n";

static char command_buffer[SCREEN_WIDTH];

static void string_shorten() {
    int i = SCREEN_WIDTH - 1;
    while (i >= 0 && (command_buffer[i] == ' ' || command_buffer[i] == '\r')) {
        command_buffer[i] = '\0';
        i--;
    }
}

static void get_last_line() {
    memset(command_buffer, 0, SCREEN_WIDTH);

    uint16_t *video_memory = (uint16_t*) VGA_ADDRESS;
    int row;
    int line_start;
    
    if (cursor_position == 0) {
        row = LAST_ROW;
    } else {
        row = (cursor_position / SCREEN_WIDTH) - 1;   
    }
    line_start = row * SCREEN_WIDTH;
    
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        uint16_t entry = video_memory[line_start + i];
        char c = (char)(entry & 0xFF);
        command_buffer[i] = c;
    }
    string_shorten();
}

// For shell
ShellCommand_t get_shell_command() {
    get_last_line();
    int i = 0;

    char* first_word = malloc(strlen(launch_stub) + 1);
    if (first_word == NULL) {
        printf("Heap memory allocation failed\n");
        return NO_COMMAND;
    }
   
    for (i = 0; i < (int) (strlen(launch_stub)); i++) {
        first_word[i] = command_buffer[i];
    }

    first_word[i] = '\0';

    if (strcmp(first_word,launch_stub) != 0) {
        printf("Command must begin with %s\n", launch_stub);
        free(first_word);
        return NO_COMMAND;
    }
    free(first_word);

    if (command_buffer[i] == '\0') {
        printf("No command given\n");
        return NO_COMMAND;
    }
    
    if (command_buffer[i] != ' ') {
        printf("No space after %s\n", launch_stub);
        return NO_COMMAND;
    }
    
    i++;

    if ((strcmp(command_buffer + i, info_stub) == 0) || (strcmp(command_buffer + i, help_stub) == 0)) 
        return LOAD_STATIC_SCREEN;
    
    
    if(strcmp(command_buffer + i, clear_stub) == 0) 
        return CLEAR_SCREEN;
    
    
    if (strcmp(command_buffer + i, music_player_stub) == 0) 
        return LOAD_MUSIC_PLAYER;

    if (strcmp(command_buffer + i, art_launch_stub) == 0)
        return LOAD_ART;
    
    if (strcmp(command_buffer + i, heap_print_stub) == 0)
        return HEAP_PRINT;
    
    printf("No valid command given\n");
    return NO_COMMAND;
    
}

char* get_shell_command_string() {
    return command_buffer + (int) strlen(launch_stub) + 1;
}

// For music player
Music_Command_t get_music_command() {
    get_last_line();
    int i = 0;

    char* first_word = malloc(strlen(music_command_stub) + 1);
    if (first_word == NULL) {
        printf("Heap memory allocation failed\n");
        return NO_MUSIC_COMMAND;
    }
   
    for (i = 0; i < (int) (strlen(music_command_stub)); i++) {
        first_word[i] = command_buffer[i];
    }

    first_word[i] = '\0';

    if (strcmp(first_word,music_command_stub) != 0) {
        printf("Command must begin with %s\n", music_command_stub);
        free(first_word);
        return NO_MUSIC_COMMAND;
    }
    free(first_word);

    if (command_buffer[i] == '\0') {
        printf("No command given\n");
        return NO_MUSIC_COMMAND;
    }
    
    if (command_buffer[i] != ' ') {
        printf("No space after %s\n", music_command_stub);
        return NO_MUSIC_COMMAND;
    }
    
    i++;

    if (strcmp(command_buffer + i, help_stub) == 0)
        return LOAD_MUSIC_PLAYER_HELP;

    if (strcmp(command_buffer + i, music_command_list) == 0)
        return LIST_SONGS;

    if (strcmp(command_buffer + i, clear_stub) == 0)
        return CLEAR_SCREEN_MUSIC;
    
    if (strcmp(command_buffer + i, music_command_exit) == 0)
        return EXIT;

    // Check for play or info
    if(strncmp(command_buffer + i, music_command_play, strlen(music_command_play)) == 0) {
        return PLAY_SONG;
    }

    if(strncmp(command_buffer + i, music_command_info, strlen(music_command_info)) == 0) {
        return SHOW_INFO;
    }

    printf("No valid command given\n");
    return NO_MUSIC_COMMAND;
       
}   

char* get_music_command_string(Music_Command_t cmd) {
    switch (cmd) {
        case PLAY_SONG: {
            int i = (int) (strlen(music_command_stub) + strlen(music_command_play) + 2);
            return command_buffer + i;
            break;
        }
            
        case SHOW_INFO: {
            int i = (int) (strlen(music_command_stub) + strlen(music_command_info) + 2);
            return command_buffer + i;
            break;
        }
            
        default:
            break;
    }
    return NULL;
}

// For art mode
Art_Command_t get_art_command() {
    get_last_line();
    int i = 0;

    char* first_word = malloc(strlen(art_command_stub) + 1);
    if (first_word == NULL) {
        printf("Heap memory allocation failed\n");
        return NO_ART_COMMAND;
    }
   
    for (i = 0; i < (int) (strlen(art_command_stub)); i++) {
        first_word[i] = command_buffer[i];
    }

    first_word[i] = '\0';

    if (strcmp(first_word,art_command_stub) != 0) {
        printf("Command must begin with %s\n", art_command_stub);
        free(first_word);
        return NO_ART_COMMAND;
    }
    free(first_word);

    if (command_buffer[i] == '\0') {
        printf("No command given\n");
        return NO_ART_COMMAND;
    }
    
    if (command_buffer[i] != ' ') {
        printf("No space after %s\n", art_command_stub);
        return NO_ART_COMMAND;
    }
    
    i++;

    if (strcmp(command_buffer + i, help_stub) == 0)
        return LOAD_ART_HELP;

    if (strcmp(command_buffer + i, art_command_exit) == 0)
        return ART_EXIT;

    if (strcmp(command_buffer + i, clear_stub) == 0)
        return CLEAR_SCREEN_ART;

    if (strcmp(command_buffer + i, art_command_list) == 0)
        return LIST_DRAWINGS;
    
    if (strncmp(command_buffer + i, art_command_new, strlen(art_command_new)) == 0)
        return NEW_DRAWING;
    
    if (strncmp(command_buffer + i, art_command_load, strlen(art_command_load)) == 0)
        return LOAD_DRAWING;
    
    if (strncmp(command_buffer + i, art_command_delete, strlen(art_command_delete)) == 0)
        return DELETE_DRAWING;
    
    

    printf("No valid command given\n");
    return NO_ART_COMMAND;
       
}

char* get_art_command_string(Art_Command_t cmd) {
    switch (cmd) {
        case NEW_DRAWING: {
            int i = (int) (strlen(art_command_stub) + strlen(art_command_new) + 2);
            return command_buffer + i;
            break;
        }
            
        case LOAD_DRAWING: {
            int i = (int) (strlen(art_command_stub) + strlen(art_command_load) + 2);
            return command_buffer + i;
            break;
        }

        case DELETE_DRAWING: {
            int i = (int) (strlen(art_command_stub) + strlen(art_command_delete) + 2);
            return command_buffer + i;
            break;
        }
            
        default:
            break;
    }
    return NULL;
}