#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <libc/stdio.h>
#include <libc/memory.h>
#include <multiboot2.h>
#include "gdt.h"
#include "arch/i386/idt.h"
#include "keyboard.h"
#include "libc/paging.h"
#include "pmalloc.h"

extern uint32_t end;

#define HEAP_SIZE 512 * 1024 // 512 KB

void test_handler(void *data) {
    printf("ISR 0 triggered!\n");
}


struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    init_gdt();
    init_idt();
    printf("Hello, World! Magic number: %d\n", magic);
    enable_interrupts();
    init_keyboard();

    printf("Setting up Paging... \n");
    init_paging();
    enable_paging();
    printf("Paging enabled! \n");
    pmalloc(2);
    printf("Pmalloc works\n");
    
    init_kernel_memory(&end);
    
    char* heap_test = (char*)malloc(128);
    printf("malloc returned: 0x%x\n", (uint32_t)heap_test);
    print_memory_layout();

   /* heap_init((void*)&end, HEAP_SIZE);
    print_heap();*/

    //register_int_handler(0, test_handler, NULL);

    // Trigger interrupt manually to test
    //__asm__ volatile ("int $0");
    //printf("%d", 1/0);
    //__asm__ volatile ("int $1");
    //__asm__ volatile ("int $2");
    while (1){} 

    return 0;

}