
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/multiboot2.h"
#include "libc/teminal.h"  
#include "libc/gdt.h"
#include "libc/idt.h"
#include "libc/keyboard.h"
#include "libc/vga.h"
#include "libc/io.h"
#include "libc/pit.h"
#include "libc/memory.h"
#include "libc/song.h"
#include "libc/snake.h"   

extern uint32_t end;

int main(uint32_t magic, struct multiboot_info* mb_info_addr) 
{
    
    init_gdt();
    init_idt();
    initKeyboard();           
    __asm__ volatile("sti");  

    
    init_kernel_memory((uint32_t)&end);
    paging_init();
    print_memory_layout();

    void* mem1 = malloc(1000);
    void* mem2 = malloc(500);
    kprint("Allocated memory blocks at %x and %x\n", mem1, mem2);

    
    init_pit();

    
    
    snake_run();

    
    return 0;
}
