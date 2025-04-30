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
            printf("Malloc adresse: 0x%x\n", (uint32_t)some_memory);

            void* memory2 = malloc(54321); 
            printf("Malloc adresse: 0x%x\n", (uint32_t)memory2);
            
            void* memory3 = malloc(13331);
            printf("Malloc adresse: 0x%x\n", (uint32_t)memory3);
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
            printf("Press 'q' to quit.\n");
            int counter = 0;

            while(1) {
                printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
                sleep_busy(1000);
                printf("[%d]: Slept using busy-waiting.\n", counter++);

                char choice_menu[2];
                get_input(choice_menu, sizeof(choice_menu));
                if(choice_menu[0] == 'q') break;

            }
           
        } 
        
        else if (choice[0] == '2') {
            printf("Press 'q' to quit.\n");

            char choice_menu[1];
            get_input(choice_menu, sizeof(choice_menu));

            int counter = 0;
            while(choice[0] != 'q') {
                
                printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
                sleep_busy(1000);
                printf("[%d]: Slept using busy-waiting.\n", counter++);
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