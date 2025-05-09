#include "libcpp/stddef.h"
#include "musicplayer/song.h"

extern "C"  
{
    #include "libc/stdio.h"
    #include "fun/menu.h"
    
    int kernel_main(void);
}

int kernel_main() {
    printf("Print from kernel_main\n");

    while (1) {
        init_menu();
    }
    

    for(;;) {
        // Halt CPU until next interrupt
        asm volatile("hlt");
    }
    return 0;
}