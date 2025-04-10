#include "state/state.h"
#include "music_player/song_library.h"
#include "music_player/song_player.h"
#include "state/shell_command.h"
#include "interrupts/keyboard/keyboard.h"
#include "main_menu/main_menu.h"
#include "screens/screens.h"
#include "interrupts/pit.h"

#include "terminal/print.h"
#include "terminal/cursor.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"

static volatile SystemState current_state = NOT_USED;
static volatile SystemState previous_state = NOT_USED;

static bool playing = false;

static bool same_state_check(void) {
    return current_state == previous_state;
}

SystemState get_current_state(void) {
    return current_state;
}

void change_state(SystemState new_state) {
    if (new_state != current_state) {
        current_state = new_state;
    }
}

void update_state(void) {
    switch (current_state)
    {
    case NOT_USED: {
        change_state(START_SCREEN);
        break;
    }

    case START_SCREEN: {
        if (same_state_check()) {
            if (keyboard_has_char()) {
                char c = keyboard_get_char();
                if (c == '\r') {
                    change_state(SHELL);
                }
            }
            break;
        }
        previous_state = current_state;
        start_screen_reveal();
        break;
    }
    case SHELL: {
        if (same_state_check()) {
            if (keyboard_has_char()) {
                char c = keyboard_get_char();
                if (c != '\x1B') {
                    printf("%c", c);
                    if (c == '\r') {
                        ShellCommand_t cmd = get_shell_command();
                        switch (cmd) {
                            case LOAD_STATIC_SCREEN:
                                change_state(STATIC_SCREEN);
                                break;
                            case LOAD_MUSIC_PLAYER:
                                change_state(MUSIC_PLAYER);
                                break;
                            case CLEAR_SCREEN:
                                clearTerminal();
                                break;
                            default:
                                break;
                        }
                    }
                } else {
                    if (keyboard_has_char()) {
                        char d = keyboard_get_char();
                        if (d == 'D') {
                            move_cursor_left();
                        } else if (d == 'C') {
                            move_cursor_right();
                        }
                    }
                }
            }
            break;
        }

        previous_state = current_state;
        clearTerminal();
        break;
    }
    case STATIC_SCREEN: {
        if (same_state_check()) {
            if (keyboard_has_char()) {
                char c = keyboard_get_char();
                if (c == '\x1B' && keyboard_has_char() == false)  {
                    change_state(SHELL);
                }
            }
            break;
        }
        previous_state = current_state;
        char* command = get_shell_command_string();
        if (command != NULL) {
            if (strcmp(command, info_stub) == 0) {
                print_about_screen();
            } else if (strcmp(command, help_stub) == 0) {
                print_command_help();
            }
        }
        break;
    }

    case MUSIC_PLAYER_HELP: {
        if (same_state_check()) {
            if (keyboard_has_char()) {
                char c = keyboard_get_char();
                if (c == '\x1B' && keyboard_has_char() == false)  {
                    clearTerminal();
                    change_state(MUSIC_PLAYER);
                }
            }
            break;
        }
        previous_state = current_state;
        print_music_player_help();
        break;
    }
    
    case MUSIC_PLAYER: {
        if (same_state_check()) {
            if (keyboard_has_char()) {
                char c = keyboard_get_char();
                if (c != '\x1B') {
                    printf("%c", c);
                    if (c == '\r') {
                        Music_Command_t cmd = get_music_command();
                        switch (cmd) {
                            case LOAD_MUSIC_PLAYER_HELP:
                                change_state(MUSIC_PLAYER_HELP);
                                break;
                            case LIST_SONGS:
                                list_songs();
                                break;
                            case CLEAR_SCREEN_MUSIC:
                                clearTerminal();
                                break;
                            case PLAY_SONG: {                                                          
                                    if (!playing) {
                                        playing = true;
                                        char* command = get_music_command_string(PLAY_SONG);
                                        int song_index = get_song_index(command);
                        
                                        if (song_index == -1) {
                                            printf("Song with code %s not found.\n", command);
                                            playing = false;
                                            break;
                                        }
                                        playSong(songList[song_index]);
                                        playing = false;
                                    }
                                }
                                break;
                            case SHOW_INFO:
                                char* command = get_music_command_string(SHOW_INFO);
                                int song_index = get_song_index(command);
                                if (song_index == -1) {
                                    printf("Song with code %s not found.\n", command);
                                    break;
                                }
                                printf("Song: %s\n", songList[song_index].title);
                                printf("Artist: %s\n", songList[song_index].artist);
                                printf("Information: %s\n", songList[song_index].information);
                                printf("\n");
                                break;
                            case EXIT:
                                change_state(SHELL);
                                if (songList != NULL) {
                                    destroy_song_library();
                                }
                                printf("Exiting music player...\n");
                                sleep_busy(1000);
                                break;
                            default:
                                break;
                        }
                    }
                } else {
                    if (keyboard_has_char()) {
                        char d = keyboard_get_char();
                        if (d == 'D') {
                            move_cursor_left();
                        } else if (d == 'C') {
                            move_cursor_right();
                        }
                    }
                }
            }
            break;
        }
        previous_state = current_state;
        if (!songLibraryInitialized) {
            init_song_library();
            songLibraryInitialized = true;
        }
                
        break;
    }
    
    default:
        __asm__ volatile ("hlt");
        break;
    }
}