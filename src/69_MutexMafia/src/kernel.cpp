extern "C" {
    #include "memory/malloc.h"
    #include "memory/paging.h"
    #include "utils/utils.h"
    #include "idt/idt.h"
    #include "io/keyboard.h"
    #include "io/printf.h"
    #include "pit/pit.h"
    #include "music/songplayer.h"
    #include "monitor/monitor.h"
    #include "game/game.h"
}
extern "C" int kernel_main(void);







    int kernel_main(){
        init_monitor();
        mafiaPrint("Kernel main function started\n");
        init_pit();
        //while (1){};
        


 
        while(1){
       print_menu();
    char input [50];
    get_input(input, sizeof(input));


    switch (input[0]) {
        case '1':
            mafiaPrint("\nHello World!\n");
            break;
        case '2':
            mafiaPrint("\n");
            print_memory_layout();
            break;
        case '3':
            {
                int input_size = 0;
                mafiaPrint("\nEnter the size of memory to allocate: ");
                get_input(input, sizeof(input));
                input_size = stoi(input);
                void* address = malloc(input_size); 
                break;
            }
        case '4':
            mafiaPrint("\nplay song\n");
            song_menu();
            break;
        case '5':
            mafiaPrint("\nPlaying Mafia Bird...\n");
            play_game();
            break;
        case '6':
            mafiaPrint("\nExiting...\n");
            return 0;
            //break;
        default:
            mafiaPrint("\nInvalid option. Please try again.\n");
    }
}
        





        while(1) {}



        


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

    }
