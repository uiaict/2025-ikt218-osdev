#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include "libc/monitor.h"
#include "libc/memory/memory.h"
#include "libc/pit.h"
#include "libc/music.h"
#include <multiboot2.h>
#include "arch/i386/gdt/gdt.h"
#include "arch/i386/idt/idt.h"
#include "libc/frequencies.h"
#include "libc/song.h"

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    
    //Initialize gdt from gdt.h
    init_gdt();

    //Initialize idt from idt.h
    init_idt();

    //Initialize the keyboard
    init_keyboard();

    //Enable interrupts
    __asm__ volatile ("sti");

    //Interupt 0 test 
    asm("int $0x0");

    init_kernel_memory(&end); // <------ THIS IS PART OF THE ASSIGNMENT

    init_paging();

    print_memory_layout();
    
    init_pit(); // <------ THIS IS PART OF THE ASSIGNMENT


    play_song(music_1, MUSIC_1_LENGTH);


    
    //infinite loop to keep the kernel running
    while (1) {
        __asm__ volatile ("hlt");
    }

    return 0;

}