#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include "libc/util.h"
#include "libc/vga.h"
#include "libc/gdt.h"
#include "libc/idt.h"
#include "libc/keyboard.h"
#include "libc/PitTimer.h"
#include "libc/song.h"
#include <multiboot2.h>



struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};
uint32_t counter;

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    Reset();
    initGdt();
    initIdt();
    initKeyboard();
    initTimer();
    playSong(music_6, sizeof(music_6));


    return 0;

}