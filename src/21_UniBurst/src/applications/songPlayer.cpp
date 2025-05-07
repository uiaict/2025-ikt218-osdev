// Source file for songPlayer based on the Per-Arne Andersen's pseudo code from: https://perara.notion.site/Assignment-5-Music-Player-ef529c6c32724b7ab626297d0dc9b34d
extern "C" {
    
    #include "pit.h"
    #include "libc/stdio.h"
    #include "io.h"
}

#include "applications/song.h"
#include "applications/frequencies.h"



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


void playSongImpl(Song *song) {
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
    playSongImpl(song);
}


SongPlayer* createSongPlayer() {
    auto* player = new SongPlayer();                
    player->playSong = playSongImpl;               
    return player;                                    
}

