extern "C" {
    #include "memory/malloc.h"
    #include "memory/paging.h"
    #include "utils/utils.h"
    #include "idt/idt.h"
    #include "io/keyboard.h"
    #include "io/printf.h"
    #include "pit/pit.h"
    #include "music/songplayer.h"
}
extern "C" int kernel_main(void);



    SongPlayer* create_song_player() {
        SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
        player->play_song = play_song_impl;
        return player;
    }


    int kernel_main(){
        mafiaPrint("Kernel main function started\n");
        init_pit(); 
        
        Song songs[] = {
            {starwars_theme, sizeof(starwars_theme) / sizeof(Note)}
        };

        uint32_t n_songs = sizeof(songs) / sizeof(Song);
        SongPlayer* player = create_song_player();

        while(1) {
            for(uint32_t i = 0; i < n_songs; i++) {
                mafiaPrint("Playing Song...\n");
                player->play_song(&songs[i]);
                mafiaPrint("Finished playing the song.\n");
            }
        }
        // free(player);   
        




        /*
        while(true){
            int counter = 0;
            mafiaPrint("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
            sleep_busy(1000);
            mafiaPrint("[%d]: Slept using busy-waiting.\n", counter++);
    
            mafiaPrint("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
            sleep_interrupt(1000);
            mafiaPrint("[%d]: Slept using interrupts.\n", counter++);
        }; */
        while (1) {}
    }
