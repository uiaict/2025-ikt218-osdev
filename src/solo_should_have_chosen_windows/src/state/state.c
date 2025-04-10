#include "state/state.h"
#include "state/shell_command.h"
#include "draw/art.h"
#include "music_player/song_library.h"
#include "music_player/song_player.h"
#include "interrupts/keyboard/keyboard.h"
#include "interrupts/pit.h"
#include "main_menu/main_menu.h"
#include "screens/screens.h"
#include "memory/heap.h"

#include "terminal/print.h"
#include "terminal/cursor.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"

static volatile SystemState current_state = NOT_USED;
static volatile SystemState previous_state = NOT_USED;

static bool playing = false;
static Drawing* current_drawing = NULL;

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
                            case LOAD_ART:
                                change_state(ART);
                                break;    
                            case CLEAR_SCREEN:
                                clearTerminal();
                                break;
                            case HEAP_PRINT:
                                print_heap();
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
    case ART: {
        if (same_state_check()) {
            if (keyboard_has_char()) {
                char c = keyboard_get_char();
                if (c != '\x1B') {
                    printf("%c", c);
                    if (c == '\r') {
                        Art_Command_t cmd = get_art_command();
                        switch (cmd) {
                            case LOAD_ART_HELP:
                                change_state(ART_HELP);
                                break;
                            case ART_EXIT:
                                change_state(SHELL);
                                printf("Exiting art software...\n");
                                sleep_busy(750);
                                break;
                            case CLEAR_SCREEN_ART:
                                clearTerminal();
                                break;
                            case NEW_DRAWING: {
                                ArtManager *manager = create_art_manager();
                                if (manager != NULL) {
                                    if (manager->space_available()) {
                                        char* command = get_art_command_string(NEW_DRAWING);
                                        if (command == NULL) {
                                            printf("Name not found.\n");
                                            break;
                                        }
                                        if (strlen(command) == 0) {
                                            printf("Name cannot be empty.\n");
                                            break;
                                        }
                                        manager->create_drawing(command);
                                        printf("Drawing with name %s created.\n", command);
                                        destroy_art_manager(manager);
                                        break;
                                    } else {
                                        printf("No space available for new drawing.\n");
                                    }
                                }
                            }
                                break;
                            case LOAD_DRAWING: {
                                ArtManager *manager = create_art_manager();
                                if (manager != NULL) {
                                    if (manager->drawings_exist()) {
                                        char* command = get_art_command_string(LOAD_DRAWING);
                                        if (command == NULL) {
                                            printf("Name not found.\n");
                                            break;
                                        }
                                        if (strlen(command) == 0) {
                                            printf("Name cannot be empty.\n");
                                            break;
                                        }
                                        current_drawing = manager->fetch_drawing(command);
                                        if (current_drawing != NULL) {
                                            change_state(ART_DRAWING);
                                        } else {
                                            printf("Drawing with name %s not found.\n", command);
                                        }
                                
                                        destroy_art_manager(manager);
                                        break;
                                    } else {
                                        printf("No drawings exist.\n");
                                    }
                                }
                            }
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
        break;
    }

    case ART_DRAWING: {
        if (same_state_check()) {
            if (keyboard_has_char()) {
                char c = keyboard_get_char();
                if (c != '\x1B') {
                    if (c != '\r' && c != '\n' && c != '\t')
                        printf("%c", c);
                } else {
                    if (keyboard_has_char()) {
                        char d = keyboard_get_char();
                        if (d == 'D') {
                            move_cursor_left();
                        } else if (d == 'C') {
                            move_cursor_right();
                        }
                    }
                    else {
                        ArtManager *manager = create_art_manager();
                        if (manager != NULL) {
                            manager->save_drawing(current_drawing);
                            clearTerminal();
                            printf("Drawing with name %s saved.\n", current_drawing->name);
                            change_state(ART);
                            destroy_art_manager(manager);
                            current_drawing = NULL;
                            break;
                        }
                    }
                }
            }
            break;
        }

        previous_state = current_state;
        clearTerminal();
        ArtManager *manager = create_art_manager();
        if (manager != NULL) {
            manager->print_board(current_drawing);
            destroy_art_manager(manager);
        }
        break;
    }

    case ART_HELP: {
        if (same_state_check()) {
            if (keyboard_has_char()) {
                char c = keyboard_get_char();
                if (c == '\x1B' && keyboard_has_char() == false)  {
                    clearTerminal();
                    change_state(ART);
                }
            }
            break;
        }
        previous_state = current_state;
        print_art_help();
        break;
    }
    default:
        __asm__ volatile ("hlt");
        break;
    }
}