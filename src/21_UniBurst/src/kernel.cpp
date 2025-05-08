// The memory related functions in this file are based on Per-Arne Andersen's implementation found at https://perara.notion.site/IKT218-Advanced-Operating-Systems-2024-bfa639380abd46389b5d69dcffda597a

extern "C"
{
    #include "libc/stdio.h"
    #include "libc/string.h"
    #include "memory.h"
    #include "pit.h"
    #include "io.h"
    #include "macros.h"
    #include "keyboard.h"

    int kernel_main(void);
}

#include "applications/demos.h"
#include "applications/song.h"


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



int kernel_main(){
    // loading screen
    disableCursor();
    printf("Starting OS [");
    videoMemory[cursorPos + 60] = ']';
    
    for (int i = 0; i < 30; i++){
        putchar('|');
        sleepInterrupt(100);
    }

    cursorVertical;  
    clearScreen();

    charBuffer[0] = 0;
    bufferIndex = 0;
    
    // full group name us UniBurst but its been shortened to UB OS to fit on screen
    printf("U     U  BBBBB        OOO    SSSSS \n");
    printf("U     U  B    B      O   O  S      \n");
    printf("U     U  B    B      O   O  S      \n");
    printf("U     U  BBBBB       O   O   SSSS  \n");
    printf("U     U  B    B      O   O       S \n");
    printf("U     U  B    B      O   O       S \n");
    printf(" UUUUU   BBBBB        OOO    SSSSS \n");

   
    printf("\n              Created by Saw John Thein, Max Meyer Hellwege, Tamim Norani\n");

    printf("\nType 'help' for a list of commands to run demos or press 'esc' to enter drawing mode.\n");
    printf("Some demos will require reboot\n");



    // demo loop
    while (true){
        char input[100];
        scanf("%s", input);

        // prints the help menu
        if (strcmp(input, "help") == 0){ 
            printf("Available commands:\n");
            printf("'print' - Runs printf demo\n");
            printf("'memory' - Runs memory demo\n");
            printf("'pagefault' - Runs page fault demo\n");
            printf("'pit' - Runs pit demo\n");
            printf("'isr' - Runs isr demo\n");
            printf("'song' - Runs song demo\n");
            printf("'piano' - Runs keyboard piano demo\n");
            printf("'exit' - Exits demo mode and allows for free typing\n");
        }

        // runs printf demo
        else if (strcmp(input, "print") == 0){
            printDemo();
        }

        // runs memory demo
        else if (strcmp(input, "memory") == 0){
            void* someMemory = malloc(12345); 
            void* memory2 = malloc(54321); 
            void* memory3 = malloc(13331);
            char* memory4 = new char[1000](); 
        }

        // runs page fault demo
        else if (strcmp(input, "pagefault") == 0){
            pageFaultDemo();
        }

        // runs pit demo
        else if (strcmp(input, "pit") == 0){
            pitDemo();
        }

        else if (strcmp(input, "isr") == 0){
            isrDemo();
        }

        // runs song demo
        else if (strcmp(input, "song") == 0){
            printf("Available songs:\n");
            printf("0. Play mariosong\n");

            printf("Enter the number of the song you want to play: ");
            int choice;
            scanf("%d", &choice);

            if (choice < 0 || choice > 1){
                printf("Invalid song number. Exiting...\n");
                continue;
            }
            
            Song* songs[] = {
                new Song{mariosong, sizeof(mariosong) / sizeof(Note)},
            };

            uint32_t nSongs = sizeof(songs) / sizeof(Song*); 

            SongPlayer* player = createSongPlayer(); 
            
            // play the songs
            if (choice == 0) {
                for(uint32_t i = 0; i < nSongs; i++){
                    printf("Playing Song...\n");
                    player->playSong(songs[i]);
                    printf("Finished playing the song.\n");
                }
            } 
    
            else {
                printf("Playing Song...\n");
                player->playSong(songs[choice - 1]);
                printf("Finished playing the song.\n");
            }
        }



        // runs the piano keyboard demo
        else if (strcmp(input, "piano") == 0) {
            keyboardPianoDemo();
        }

        // exits demo mode
        else if (strcmp(input, "exit") == 0){
            printf("Exiting demo mode. You can still enter drawing mode by pressing 'esc'\n");
            break;
        }

        else {
            printf("Unknown command. Type 'help' for a list of commands.\n");
        }
    }

    while(true);
}