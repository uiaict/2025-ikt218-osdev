#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>

#include "gdt/gdt.h"
#include "io/printf.h"
#include "idt/idt.h"
#include "io/keyboard.h"
#include "memory/malloc.h"
#include "memory/paging.h"
#include "pit/pit.h"





struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();
extern uint32_t end;


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    initGdt();
    initIdt();
    init_kernel_memory(&end);
    init_paging();
    init_pit();
    test_pit();
    
    //init_Pit

    initKeyboard();
    mafiaPrint("Wadiyatalkinabeet\n");
    mafiaPrint("\n");

    
    void* some_memory = malloc(12345); 
    void* memory2 = malloc(54321); 
    void* memory3 = malloc(13331);
    print_memory_layout();

    
    return kernel_main(); 

}