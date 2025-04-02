#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include <multiboot2.h>
#include "arch/i386/gdt/gdt.h"

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

    char* hello_world = "Hello, World!";
    size_t len = strlen(hello_world);

    //Write to video memory
    char* video_memory = (char*) 0xb8000;

    //Write hello_world to video memory
    for (size_t i = 0; i < len; i++) {
        video_memory[i * 2] = hello_world[i];
        video_memory[i * 2 + 1] = 0x07;
    }

    int noop = 0;
    return 0;

}