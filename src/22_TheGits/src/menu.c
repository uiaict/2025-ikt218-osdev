#include "libc/scrn.h"
#include "memory/memory.h"
#include "menu.h"
#include "pit/pit.h"
#include "audio/tracks.h"
#include "game/wordgame.h"

void memory_menu() {
    char choice[5];
    while (1) {
        printf("\n==== Memory Management Menu ====\n");
        printf("1: Print memory layout\n");
        printf("2: Check memory allocation\n");
        printf("q: Go back to main menu....\n");

        get_input(choice, sizeof(choice));

        if (choice[0] == '1') {
            print_memory_layout();
        } 
        
        else if (choice[0] == '2') {

            printf("Allocating 12345, 54321, and 13331 bytes of memory...\n");
            void* some_memory = malloc(12345);
            printf("Malloc adresse for 12345: 0x%x\n", (uint32_t)some_memory);

            void* memory2 = malloc(54321); 
            printf("Malloc adresse for 54321: 0x%x\n", (uint32_t)memory2);
            
            void* memory3 = malloc(13331);
            printf("Malloc adresse 13331: 0x%x\n", (uint32_t)memory3);
        } 
        
        else if (choice[0] == 'q') {
            break;
        } 
        
        else {
            printf("Invalid choice. Please try again.\n");
        }
    }
}

void pit_menu() {
    char choice[3];


    while (1) {
        printf("\n==== PIT Management Menu ====\n");
        printf("1: Test sleep busy\n");
        printf("2: Test sleep interrupt\n");
        printf("q: Go back to main menu....\n");

        get_input(choice, sizeof(choice));

        if (choice[0] == '1') {
           
            int counter = 0;
            char input[3];

            while(1) {
                printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
                sleep_busy(1000);
                printf("[%d]: Slept using busy-waiting.\n", counter++);

                printf("Press 'q' to quit or Enter to continue\n");
                get_input(input, sizeof(input));
                if(input[0] == 'q') break;
            }
        } 
        
        else if (choice[0] == '2') {
            
            int counter = 0;
            char input[3];

    
            while(1) {
                
                printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
                sleep_busy(1000);
                printf("[%d]: Slept using busy-waiting.\n", counter++);

                printf("Press 'q' to quit or Enter to continue\n");
                get_input(input, sizeof(input));
                if(input[0] == 'q') break;
            }

        } 
        
        else if (choice[0] == 'q') {
            break;
        } 
        
        else {
            printf("Invalid choice. Please try again.\n");
        }
    }
}

void play_music_menu(){
    while(1){
        printf("\n==== Music Player Menu ====\n");
        printf("1: Play Mario Theme Song\n");
        printf("2: Play Star Wars Theme Song\n");
        printf("3: Play Battlefield 1942 Theme Song\n");
        printf("q: Go back to main menu....\n");

        char choice[4];
        get_input(choice, sizeof(choice));

        if (choice[0] == '1') {
            printf("Playing Mario Theme Song...\n");
            play_music(music_1, sizeof(music_1) / sizeof(Note));
            sleep_busy(2000); // Sleep for 2 seconds before stopping
            printf("Finished playing the song.\n");
        } 
        else if (choice[0] == '2') {
            printf("Playing Star Wars Theme Song...\n");
            play_music(starwars_theme, sizeof(starwars_theme) / sizeof(Note));
            sleep_busy(2000); // Sleep for 2 seconds before stopping
            printf("Finished playing the song.\n");
        } 
        else if (choice[0] == '3') {
            printf("Playing Battlefield 1942 Theme Song...\n");
            play_music(battlefield_1942_theme, sizeof(battlefield_1942_theme) / sizeof(Note));
            sleep_busy(2000); // Sleep for 2 seconds before stopping
            printf("Finished playing the song.\n");
        } 
        else if (choice[0] == 'q' || choice[0] == 'Q') {
            printf("Exiting music player...\n");
            return;
        } else {
            printf("Invalid input. Try again.\n");
        }
    }
}

void start_game_menu() {
    while (1) {
        printf("\n==== Word Game Menu ====\n");
        printf("1: Start game\n");
        printf("2: Show highscores\n");
        printf("q: Quit game\n");
        printf("Your choice: ");

        char choice[4];
        get_input(choice, sizeof(choice));

        if (choice[0] == '1') {
            start_word_game();
        } else if (choice[0] == '2') {
            show_highscores();
        } else if (choice[0] == 'q' || choice[0] == 'Q') {
            printf("Exiting game...\n");
            return;
        } else {
            printf("Invalid input. Try again.\n");
        }
    }
}

void print_os_logo(){
    printf(" _________  __               ______   _   _           ___     ______   \n");
    printf("|  _   _  |[  |            .' ___  | (_) / |_       .'   `. .' ____ \\  \n");
    printf("|_/ | | \\_| | |--.  .---. / .'   \\_| __ `| |-'.--. /  .-.  \\| (___ \\_| \n");
    printf("    | |     | .-. |/ /__\\\\| |   ____[  | | | ( (`\\]| |   | | _.____`.  \n");
    printf("   _| |_    | | | || \\__.,\\ `.___]  || | | |, `'.'.\\  `-'  /| \\____) | \n");
    printf("  |_____|  [___]|__]'.__.' `._____.'[___]\\__/[\__) )`.___.'  \\______.' \n");
}

void print_os_greeting(){
    print_os_logo();
    printf("Welcome to TheGitsOS!\n");
    printf("Use our interacive menu to navigate through the system.\n");
    printf("\n");
    
}
void print_os_farewell(){
    print_os_logo();
    printf("Thank you for using TheGitsOS...\n");
    printf("We hope you enjoyed your experience.\n");
}