#include "music_player/frequencies.h"
#include "interrupts/speaker.h"
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

#define HEAP_SIZE 64 * 1024 // 64 KB


extern uint32_t end;

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

void fur_elise() {
    printf("Playing Fur Elise...\n");
    
    speaker_beep(E5, 150);
    sleep_busy(20);
    speaker_beep(Ds5, 150);
    sleep_busy(20);
    speaker_beep(E5, 150);
    sleep_busy(20);
    speaker_beep(Ds5, 150);
    sleep_busy(20);
    speaker_beep(E5, 150);
    sleep_busy(20);
    speaker_beep(B4, 150);
    sleep_busy(20);
    speaker_beep(D5, 150);
    sleep_busy(20);
    speaker_beep(C5, 150);
    sleep_busy(20);
    speaker_beep(A4, 150);
    sleep_busy(20); 
    speaker_beep(C4, 150);
    sleep_busy(20); 
    speaker_beep(E4, 150);
    sleep_busy(20); 
    speaker_beep(A4, 150);
    sleep_busy(20);
    speaker_beep(B4, 200);
    sleep_busy(20);
    speaker_beep(E4, 150);
    sleep_busy(20); 
    speaker_beep(A4, 150);
    sleep_busy(20);
    speaker_beep(B4, 150);
    sleep_busy(20); 
    speaker_beep(C5, 500);
    sleep_busy(20);   
    printf("Ended music...\n");
}


int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    
    gdt_init();
    idt_init();
    enable_interrupts();
    pit_init();
    printf("GDT, IDT and interrupts initialized. PIT configured.\n");

    printf("Initialising heap...\n");
    heap_init((void*)&end, HEAP_SIZE);

    paging_init();
    printf("Paging initialized.\n\n");
    
    print_heap(); 

    fur_elise();
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