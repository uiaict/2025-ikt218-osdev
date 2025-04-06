#include "gdt/gdt_function.h"
#include "terminal/print.h"
#include "interrupts/idt_function.h"
#include "memory/heap.h"

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

#include <multiboot2.h>

#define HEAP_SIZE 64 * 1024 // 64 KB

extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {

    gdt_init();
    // printf("gdt now loaded\n");

    idt_init();
    // printf("idt now loaded\n");

    enable_interrupts();
    // printf("interrupts enabled\n");

    printf("Kernel ends at: %x\nInitialising heap...\n", &end);
    heap_init((void*)&end, HEAP_SIZE);
    
    print_heap();

    char* string = malloc(6 * sizeof(char));
    if (string) {
        string[0] = 'H';
        string[1] = 'e';
        string[2] = 'l';
        string[3] = 'l';
        string[4] = 'o';
        string[5] = '\0'; // Null-terminate the string
    }
    printf("Allocated string: %s\n", string);
    print_heap();

    /* void* a = malloc(100);
    printf("Allocated 100 bytes at %p\n", a);
    print_heap();

    void* b = malloc(200);
    printf("Allocated 200 bytes at %p\n", b);
    print_heap();

    void* c = malloc(300);
    printf("Allocated 300 bytes at %p\n", c);
    print_heap();

    free(b);
    printf("Freed 200-byte block.\n");
    print_heap();

    void* d = malloc(150);
    printf("Allocated 150 bytes at %p (should reuse freed block)\n", d);
    print_heap(); */

       // Keep the CPU running forever
    while (1) {
        __asm__ volatile ("hlt");
    }
    
    return 0;

}