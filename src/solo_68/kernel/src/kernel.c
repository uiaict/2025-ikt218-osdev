#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "keyboard.h"
#include "memory.h"
#include "pit.h"
#include <stdint.h>
#include "system.h"
#include "song_player.h"
#include "shell.h"

extern uint32_t end;

void kernel_idle() {
    while (1) {
        asm volatile ("hlt");
    }
}

void kernel_entry() {
    init_gdt();
    init_idt();
    irq_install();

    terminal_initialize();
    
    keyboard_install();
    irq_install_handler(1, keyboard_callback);
    

    init_kernel_memory(&end);  // <-- initialize the heap
    print_memory_layout();     // <-- show heap addresses

    init_paging();             // <-- setup paging after memory

    init_pit();

    asm volatile ("sti");

    shell();

    //play_music();

    kernel_idle();

}

