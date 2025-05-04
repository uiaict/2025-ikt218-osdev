/* ---------------------------------------------------------------------
    * This code is adapted from Per-Arne Andersens implementation 
        https://perara.notion.site/Assignment-4-Memory-and-PIT-83eabc342fd24b88b00733a78b5a86e
    ---------------------------------------------------------------------
*/

extern "C"{
    #include "libc/stdint.h"
    #include "libc/stdbool.h"
    #include "keyboard.h"
    #include "terminal.h"
    #include "isr.h"
    #include "memory.h"
    #include "kernelUtils.h"
    #include "descTables.h"
    #include "apps/song.h"
    #include "pit.h"
    #include "apps/frequencies.h"
    
}

// Existing global operator new overloads
void* operator new(size_t size) {
    return malloc(size);
}

void* operator new[](size_t size) {
    return malloc(size);
}

// Existing global operator delete overloads
void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}

void operator delete(void* ptr, size_t size) noexcept {
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
}


extern "C" int kernel_main(void);
int kernel_main(){

    void* some_memory = malloc(12345); 
    void* memory2 = malloc(54321); 
    void* memory3 = malloc(13331);
    char* memory4 = new char[1000]();

    free(some_memory);
    free(memory2);
    free(memory3);
    delete[] memory4;
    
     Song* songs[] = {
        new Song({"Twinkle Twinkle little star", twinkle_twinkle, sizeof(twinkle_twinkle) / sizeof(Note)}),
        // new Song({"music 1", music_1, sizeof(music_1) / sizeof(Note)}),
        // new Song({"music 2",music_2, sizeof(music_2) / sizeof(Note)}),
        // new Song({"music 3",music_3, sizeof(music_3) / sizeof(Note)}),
        // new Song({"Lisa gikk til skolen",music_4, sizeof(music_4) / sizeof(Note)}),

    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song*);
    SongPlayer* player = create_song_player();
    if (player == nullptr) 
    {
        printf("Failed to create song player\n");
        return 1;
    }
    printf("Waiting for song player...\n");
    sleep_interrupt(3000);
    printf("Number of songs: %d\n", n_songs);
    for(uint32_t i = 0; i < n_songs; i++)
    {
        printf("Now playing: %s\n", songs[i]->name);
        player->play_song(songs[i]);
        printf("Finished playing the song.\n");
        printf("Waiting for next song...\n");
        sleep_interrupt(3000);
    }
    printf("Songplayer finished playing\n");
    for (uint32_t i = 0; i < n_songs; i++) {
        delete songs[i];  
    }
    delete player;  
}