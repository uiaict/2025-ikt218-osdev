#include "music_player.h"
#include "drivers/VGA/vga.h"
#include "arch/i386/GDT/util.h"
#include "libc/string.h"
#include "drivers/PIT/pit.h"
#include "menu.h"

extern void play_sound(uint32_t frequency);
extern void stop_sound();
extern void enable_speaker();
extern void disable_speaker();
extern uint32_t get_current_tick();

extern MenuState current_state;
extern uint8_t last_scancode;

bool is_playing = false;
uint8_t current_song = 0;
uint16_t current_note = 0;
uint32_t note_start_time = 0;

extern Note music_1[];
extern Note music_2[];
extern Note music_3[];
extern Note music_4[];
extern Note music_5[];
extern Note music_6[];

const char* song_names[NUM_SONGS] = {
    "Super Mario Bros.",
    "Tetris Theme",
    "Ode to Joy (Mozart)",
    "Twinkle Twinkle Little Star",
    "A Simple Melody",
    "Star Wars Theme"
};

Song songs[NUM_SONGS] = {
    { music_1, sizeof(music_1) / sizeof(Note) },
    { music_2, sizeof(music_2) / sizeof(Note) },
    { music_3, sizeof(music_3) / sizeof(Note) },
    { music_4, sizeof(music_4) / sizeof(Note) },
    { music_5, sizeof(music_5) / sizeof(Note) },
    { music_6, sizeof(music_6) / sizeof(Note) }
};

void draw_player_frame() {
    Reset();
    
    setColor(PLAYER_TITLE_COLOR, PLAYER_BG_COLOR);
    setCursorPosition(0, 0);
    
    for (int i = 0; i < TEXT_PLAYER_WIDTH; i++) {
        putCharAt(i, 0, '-', PLAYER_BORDER_COLOR, PLAYER_BG_COLOR);
    }
    
    char title[50] = "UiA Music Player";
    int title_pos = (TEXT_PLAYER_WIDTH - strlen(title)) / 2;
    setCursorPosition(title_pos, 0);
    setColor(PLAYER_TITLE_COLOR, PLAYER_BG_COLOR);
    print(title);
    
    for (int i = 1; i < TEXT_PLAYER_HEIGHT; i++) {
        putCharAt(0, i, '|', PLAYER_BORDER_COLOR, PLAYER_BG_COLOR);
        putCharAt(TEXT_PLAYER_WIDTH - 1, i, '|', PLAYER_BORDER_COLOR, PLAYER_BG_COLOR);
    }
    
    for (int i = 0; i < TEXT_PLAYER_WIDTH; i++) {
        putCharAt(i, TEXT_PLAYER_HEIGHT, '-', PLAYER_BORDER_COLOR, PLAYER_BG_COLOR);
    }
    
    setColor(PLAYER_TEXT_COLOR, PLAYER_BG_COLOR);
    setCursorPosition(3, 2);
    print("Available Songs:");
    
    for (int i = 0; i < NUM_SONGS; i++) {
        setCursorPosition(5, 4 + i);
        
        if (i == current_song) {
            setColor(PLAYER_HIGHLIGHT_COLOR, PLAYER_BG_COLOR);
            char prefix[3] = "> ";
            print(prefix);
        } else {
            setColor(PLAYER_TEXT_COLOR, PLAYER_BG_COLOR);
            char prefix[3] = "  ";
            print(prefix);
        }
        
        char num[3];
        itoa(i + 1, num, 10);
        print(num);
        print(". ");
        
        print(song_names[i]);
    }
    
    setColor(PLAYER_TEXT_COLOR, PLAYER_BG_COLOR);
    setCursorPosition(3, TEXT_PLAYER_HEIGHT - 4);
    print("Controls:");
    setCursorPosition(5, TEXT_PLAYER_HEIGHT - 3);
    print("UP/DOWN: Select Song    SPACE: Play/Pause    ESC: Exit");
    
    setColor(PLAYER_TEXT_COLOR, PLAYER_BG_COLOR);
    setCursorPosition(3, TEXT_PLAYER_HEIGHT - 6);
    print("Status: ");
    
    if (is_playing) {
        setColor(PLAYER_HIGHLIGHT_COLOR, PLAYER_BG_COLOR);
        print("Playing");
        
        setColor(PLAYER_TEXT_COLOR, PLAYER_BG_COLOR);
        setCursorPosition(3, TEXT_PLAYER_HEIGHT - 5);
        print("Now playing: ");
        setColor(PLAYER_HIGHLIGHT_COLOR, PLAYER_BG_COLOR);
        print(song_names[current_song]);
        
        setColor(PLAYER_TEXT_COLOR, PLAYER_BG_COLOR);
        setCursorPosition(40, TEXT_PLAYER_HEIGHT - 5);
        print("Progress: ");
        
        char progress[20];
        char total[10];
        char slash[2] = "/";
        itoa(current_note, progress, 10);
        itoa(songs[current_song].length, total, 10);
        
        setColor(PLAYER_HIGHLIGHT_COLOR, PLAYER_BG_COLOR);
        print(progress);
        print(slash);
        print(total);
    } else {
        setColor(PLAYER_TEXT_COLOR, PLAYER_BG_COLOR);
        print("Stopped");
    }
}

void handle_player_input() {
    uint8_t scancode = inPortB(0x60);
    bool keyReleased = scancode & 0x80;
    scancode &= 0x7F;
    
    if (!keyReleased) {
        last_scancode = scancode;
        
        switch (scancode) {
            case 0x01:
                if (is_playing) {
                    stop_sound();
                    disable_speaker();
                    is_playing = false;
                }
                current_state = MENU_STATE_MAIN;
                break;
                
            case 0x39:
                is_playing = !is_playing;
                if (!is_playing) {
                    stop_sound();
                } else {
                    enable_speaker();
                    
                    if (current_note >= songs[current_song].length) {
                        current_note = 0;
                        note_start_time = 0;
                    }
                }
                draw_player_frame();
                break;
                
            case 0x48:
                if (current_song > 0) {
                    current_song--;
                    if (is_playing) {
                        stop_sound();
                        current_note = 0;
                        note_start_time = 0;
                    }
                    draw_player_frame();
                }
                break;
                
            case 0x50:
                if (current_song < NUM_SONGS - 1) {
                    current_song++;
                    if (is_playing) {
                        stop_sound();
                        current_note = 0;
                        note_start_time = 0;
                    }
                    draw_player_frame();
                }
                break;
                
            case 0x02:
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
                {
                    uint8_t song_idx = scancode - 0x02;
                    if (song_idx < NUM_SONGS) {
                        current_song = song_idx;
                        if (is_playing) {
                            stop_sound();
                            current_note = 0;
                            note_start_time = 0;
                        }
                        draw_player_frame();
                    }
                }
                break;
                
            case 0x14:
                enable_speaker();
                play_sound(440);
                draw_player_frame();
                break;
                
            case 0x21:
                stop_sound();
                draw_player_frame();
                break;
        }
    }
}

void update_player() {
    if (!is_playing) return;
    
    Song* song = &songs[current_song];
    uint32_t current_time = get_current_tick();
    
    if (current_note < song->length) {
        Note* note = &song->notes[current_note];
        
        if (note_start_time == 0 || current_time - note_start_time >= note->duration) {
            stop_sound();
            
            if (note->frequency > 0) {
                enable_speaker();
                play_sound(note->frequency);
            } else {
                stop_sound();
            }
            
            if (current_note % 5 == 0) {
                draw_player_frame();
            }
            
            note_start_time = current_time;
            current_note++;
        }
    } else {
        stop_sound();
        current_note = 0;
        note_start_time = 0;
    }
}

void test_sound() {
    enable_speaker();
    
    play_sound(440);
    
    setCursorPosition(20, 12);
    setColor(PLAYER_HIGHLIGHT_COLOR, PLAYER_BG_COLOR);
    print("*** TESTING SOUND ***");
    
    sleep_interrupt(1000);
    
    stop_sound();
}

void init_music_player() {
    is_playing = false;
    current_song = 0;
    current_note = 0;
    note_start_time = 0;
    
    draw_player_frame();
    
    enable_speaker();
}

void music_player_loop() {
    handle_player_input();
    update_player();
    
    sleep_interrupt(20);
}