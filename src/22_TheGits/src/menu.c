#include "libc/scrn.h"
#include "memory/memory.h"
#include "menu.h"
#include "pit/pit.h"

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