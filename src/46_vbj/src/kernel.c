#include "libc/stdint.h"
#include "libc/stdarg.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "descTables.h"
#include "terminal.h"
#include "keyboard.h"
#include "memory.h" 
#include "pit.h"

extern uint32_t end; // This is defined in arch/i386/linker.ld


struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();

volatile uint32_t counter = 0;

int main(uint32_t magic, struct multiboot_info* mb_info_addr) 
{
    clear_screen();
    init_descriptor_tables();
    
    printf("Testing printf:\n");
    printf("String: %s\n", "Hello, world!");

    init_keyboard();
    printf("Keyboard initialized.\n");

    init_kernel_memory(&end); 

    init_paging();
    printf("Paging initialized.\n"); 

    print_memory_layout(); 

    init_pit();
    printf("Pit initalized!\n");
   

    /*
    while(true)
    {
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);
    }; */
   


   return kernel_main();
    
    //return 0;
}