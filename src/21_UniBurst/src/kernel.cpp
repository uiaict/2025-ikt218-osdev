// The memory related functions in this file are based on Per-Arne Andersen's implementation found at https://perara.notion.site/IKT218-Advanced-Operating-Systems-2024-bfa639380abd46389b5d69dcffda597a

extern "C"
{
#include "libc/stdio.h"
#include "memory.h"
#include "pit.h"

    int kernel_main(void);
}

#include "songPlayer/song.h"



void *operator new(size_t size)
{
    return malloc(size);
}

void *operator new[](size_t size)
{
    return malloc(size);
}


void operator delete(void *ptr) noexcept
{
    free(ptr);
}

void operator delete[](void *ptr) noexcept
{
    free(ptr);
}


void operator delete(void *ptr, size_t size) noexcept
{
    (void)size; 
    free(ptr);
}

void operator delete[](void *ptr, size_t size) noexcept
{
    (void)size; 
    free(ptr);
}

int kernel_main()
{
    /*
    void* someMemory = malloc(12345);
    void* memory2 = malloc(54321);
    void* memory3 = malloc(13331);
    char* memory4 = new char[1000]();

    int counter = 0;

     while(true){
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleepBusy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleepInterrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);
    };
    */
    // Create an array of songs
    Song *songs[] = {
        new Song{mariosong, sizeof(mariosong) / sizeof(Note)},
    };

    uint32_t nSongs = sizeof(songs) / sizeof(Song *); 

    SongPlayer *player = createSongPlayer();

    // Play the song
    while (true)
    {
        for (uint32_t i = 0; i < nSongs; i++)
        {
            printf("Playing Song...\n");
            player->playSong(songs[i]);
            printf("Finished playing the song.\n");
        }
    }

    while (true)
    {
    }
}