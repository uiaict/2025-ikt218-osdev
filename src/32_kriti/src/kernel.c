#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include <kprint.h>
#include <gdt.h>
#include <pic.h>
#include <idt.h>

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    const char *str = "Hello World";
    char *video_memory = (char*) 0xb8000;
    for (int i = 0; str[i] != '\0'; i++) {
        video_memory[i*2] = str[i];
        video_memory[i*2 + 1] = 0x07;
    }

    kprint("Loading GDT...\n");
    init_gdt();
    kprint("GDT loaded\n");
    return 0;

}