extern "C" {
    #include "memory/malloc.h"
    #include "memory/paging.h"
    #include "utils/utils.h"
    #include "idt/idt.h"
    #include "io/keyboard.h"
    #include "io/printf.h"


    int kernel_main(){
        mafiaPrint("Kernel main function started\n");
        while (1) {}
    }
}