#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <multiboot2.h>
#include "gdt/gdt.h"
#include <libc/stdarg.h>
#include "vga/vga.h"
#include "idt/idt.h"
#include "keyboard/keyboard.h"
#include "PIT/PIT.h"

extern uint32_t end; // This is defined in arch/i386/linker.ld
uint32_t teller = 0;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    initGdt();
    reset();
    initIdt();
    initKeyboard();
    enable_cursor(0,15);
    initPit();
    printf("=======================================================\n");
    printf("      .___                                             \n");
    printf("    __| _/____   ____   _____       ____  ______       \n");
    printf("   / __ |/  _ \\ /  _ \\ /     \\     /  _ \\/  ___/   \n");
    printf("  / /_/ (  <_> |  <_> )  Y Y  \\   (  <_> )___ \\      \n");
    printf("  \\____ |\\____/ \\____/|__|_|  /____\\____/____  >   \n");
    printf("       \\/                   \\/_____/         \\/     \n");
    printf("    Developed by a bunch of retards for IKT218         \n");
    printf("                 Welcome to hell!                      \n");
    printf("               Aris, Marcus, Albert                    \n");
    printf("=======================================================\n");
    while(true){
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", teller);
        sleepBusy(2000);
        printf("[%d]: Slept using busy-waiting.\n", teller++);

        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", teller);
        sleepInterrupt(2000);
        printf("[%d]: Slept using interrupts.\n", teller++);
    };
    return 0;
}