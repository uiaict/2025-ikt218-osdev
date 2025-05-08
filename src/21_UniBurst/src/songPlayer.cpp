// Source file for songPlayer based on the Per-Arne Andersen's pseudo code from: https://perara.notion.site/Assignment-5-Music-Player-ef529c6c32724b7ab626297d0dc9b34d
extern "C" {
    
    #include "pit.h"
    #include "libc/stdio.h"
    #include "io.h"
    #include "keyboard.h"
}

#include "song.h"
#include "frequencies.h"



void enableSpeaker() {
    
     uint8_t state = inb(PC_SPEAKER_PORT);             
                                  
    if ((state & 3) != 3) {                             
        outb(PC_SPEAKER_PORT, state | 3);               
    }
   
}


void disableSpeaker() {
    uint8_t state = inb(PC_SPEAKER_PORT);               

    if ((state & 3) != 0) {                            
        outb(PC_SPEAKER_PORT, state & 0xFC);            
    }
}


void playSound(uint32_t frequency) {

    if (frequency == 0) {                             
        return;
    }

    uint16_t divisor = PIT_BASE_FREQUENCY / frequency;  

    outb(PIT_CMD_PORT, 0xB6);                           

    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));          
    outb(PIT_CHANNEL2_PORT, (uint8_t)((divisor >> 8) & 0xFF));      

    uint8_t state = inb(PC_SPEAKER_PORT);               
    state |= 0b11;                                     
    outb(PC_SPEAKER_PORT, state);                       
}   


void stopSound() {
    uint8_t state = inb(PC_SPEAKER_PORT);             

    state &= ~0b10;                                    

    outb(PC_SPEAKER_PORT, state);                   
}


const char* getNoteName(uint32_t frequency) {
    for (size_t i = 0; i < sizeof(noteNames) / sizeof(NoteName); i++) {
        if (noteNames[i].frequency == frequency) {
            return noteNames[i].name;
        }
    }

    return "Unknown";
}


void playmariosong(Song *song) {
    enableSpeaker();                                    
    for (size_t i = 0; i < song->length; i++) {
        Note* note = &song->notes[i];

        
        const char* noteName = getNoteName(note->frequency);    

       
        printf("Playing note %s with frequency %u Hz for %u ms\n", noteName, note->frequency, note->duration);


        playSound(note->frequency);                    
        sleepInterrupt(note->duration);                
        stopSound();                                  
    }

    
    disableSpeaker();                                           
}

void playSong(Song *song) {
    playmariosong(song);
}


SongPlayer* createSongPlayer() {
    auto* player = new SongPlayer();                
    player->playSong = playmariosong;               
    return player;                                    
}



void keyboardPianoDemo() {
    clearScreen();
    
    // the piano ui
    printf("===== Keyboard Piano Demo =====\n\n");
    printf("Press keys 1-8 to play notes:\n");
    printf("1    2    3    4    5    6    7    8\n");
    printf("C4   D4   E4   F4   G4   A4   B4   C5\n\n");
    printf("Press ESC to exit demo\n\n");
    printf("Currently playing: [No note]\n");
    
    int noteDisplayPos = cursorPos - 13;
    
    enableSpeaker();
    
    bool running = true;
    uint8_t currentKey = 0;
    bool keyPressed = false;
    
    const uint32_t pianoFreqs[8] = {
        C4, D4, E4, F4, G4, A4, B4, C5
    };

    const char* pianoNoteNames[8] = {
        "C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5"
    };
    
    while (running) {
        uint8_t scanCode = checkKeyInput();
        
        if (scanCode != 0 && !keyPressed) {
            keyPressed = true;
            
            // Convert scan code to key number (1-8)
            switch (scanCode) {
                case 0x02: // Scan code for '1'
                    currentKey = 1;
                    break;
                case 0x03: // Scan code for '2'
                    currentKey = 2;
                    break;
                case 0x04: // Scan code for '3'
                    currentKey = 3;
                    break;
                case 0x05: // Scan code for '4'
                    currentKey = 4;
                    break;
                case 0x06: // Scan code for '5'
                    currentKey = 5;
                    break;
                case 0x07: // Scan code for '6'
                    currentKey = 6;
                    break;
                case 0x08: // Scan code for '7'
                    currentKey = 7;
                    break;
                case 0x09: // Scan code for '8'
                    currentKey = 8;
                    break;
                case 0x01: // ESC key to exit
                    running = false;
                    break;
                default:
                    currentKey = 0;
                    break;
            }
            
            // if a valid key is pressed
            if (currentKey >= 1 && currentKey <= 8) {
                uint32_t freq = pianoFreqs[currentKey - 1];
                playSound(freq);
                
                int tempPos = cursorPos;
                cursorPos = noteDisplayPos;
                printf("%-6s", pianoNoteNames[currentKey - 1]);
                cursorPos = tempPos;
            }
        }
        
        if (scanCode == 0 && keyPressed) {
            keyPressed = false;
            
            stopSound();

            int tempPos = cursorPos;
            cursorPos = noteDisplayPos;
            printf("No note");
            cursorPos = tempPos;
        }
        
        sleepInterrupt(10);
    }
    
    disableSpeaker();
    
    clearScreen();
    printf("Keyboard Piano Demo Ended\n");
}