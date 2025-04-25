extern "C" {
    #include "memory/malloc.h"
    #include "memory/paging.h"
    #include "utils/utils.h"
    #include "idt/idt.h"
    #include "io/keyboard.h"
    #include "io/printf.h"
    #include "pit/pit.h"

    int kernel_main(){
        mafiaPrint("Kernel main function started\n");
        init_pit(); 
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
}
