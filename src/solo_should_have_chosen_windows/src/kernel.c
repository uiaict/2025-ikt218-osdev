#include "state/state.h"

#include "music_player/song_player.h"
#include "music_player/song_library.h"

#include "gdt/gdt_function.h"
#include "terminal/print.h"
#include "interrupts/idt_function.h"
#include "memory/heap.h"
#include "memory/paging.h"
#include "interrupts/pit.h"

#include "libc/io.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

#include <multiboot2.h>

#define HEAP_SIZE 512 * 1024 // 512 KB


extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    
    gdt_init();
    idt_init();
    keyboard_flush_buffer();
    enable_interrupts();
    pit_init();
    printf("GDT, IDT and interrupts initialized. PIT configured.\n");

    printf("Initialising heap...\n");
    heap_init((void*)&end, HEAP_SIZE);
    printf("Heap initialized.\nHeap info:\n");
    uint32_t heap_start = (uint32_t)&end;
    uint32_t heap_end = heap_start + HEAP_SIZE;

    printf("\nHeap start: %x\n", heap_start);
    printf("Heap end: %x\n", heap_end);
    print_heap();

    paging_init();
    printf("Paging initialized.\n\n");
    
    sleep_busy(10);
    
       // Keep the CPU running forever
    while (1) {
       update_state();
    }
    
    return 0;

}