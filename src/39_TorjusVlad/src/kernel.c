#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <libc/stdio.h>
#include <libc/memory.h>
#include <multiboot2.h>
#include "gdt.h"
#include "arch/i386/idt.h"
#include "keyboard.h"

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

    heap_init((void*)&end, HEAP_SIZE);
    print_heap();
    //register_int_handler(0, test_handler, NULL);

    // Trigger interrupt manually to test
    //__asm__ volatile ("int $0");
    //printf("%d", 1/0);
    //__asm__ volatile ("int $1");
    //__asm__ volatile ("int $2");
    while (1){} 

    return 0;

}