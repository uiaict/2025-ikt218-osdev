#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include <kprint.h>
#include <gdt.h>
#include <pic.h>
#include <idt.h>
#include <keyboard.h>
#include <isr.h>
#include <irq.h>

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

    pic_init();

  // Initialize IDT
  kprint("Initializing IDT...\n");
  idt_init();
  kprint("IDT initialized\n");

  kprint("Initializing IRQ...\n");
    irq_init();

  kprint("Initializing Keyboard Logger...\n");
    keyboard_init();
    kprint("Keyboard Logger initialized\n");

   
__asm__ volatile("sti");

kprint("Press key to see input\n");
while (1) {

    // Check for data directly (polling method)
   /* if (inb(0x64) & 1) {
        uint8_t scancode = inb(0x60);
        kprint("Polled scancode: ");
        kprint_hex(scancode);
        kprint("\n");
    }
    
    // Also process any characters that might have been added by the interrupt handler
    process_keyboard_input();*/
}




    return 0;

}