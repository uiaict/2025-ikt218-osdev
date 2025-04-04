#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include <multiboot2.h>
#include "arch/i386/gdt/gdt.h"
#include "arch/i386/idt/idt.h"

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


    //Test before interrupt
    char* before = "Before!";
    size_t len = strlen(before);
    char* video_memory = (char*) 0xb8000;
    for (size_t i = 0; i < len; i++) {
        video_memory[i * 2] = before[i];
        video_memory[i * 2 + 1] = 0x07;
    }

    //Call interrupt 0x0
    //asm("int $0x0");

    //Test after interrupt
    char* after = "After!";
    size_t len1 = strlen(after);
    char* video_memory1 = (char*) 0xb8000;
    for (size_t i = 0; i < len1; i++) {
        video_memory1[i * 2] = after[i];
        video_memory1[i * 2 + 1] = 0x07;
    }
    return 0;

}