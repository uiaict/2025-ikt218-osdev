#include "i386/keyboard.h"
#include "i386/descriptorTables.h"
#include "i386/interruptRegister.h"
#include "i386/monitor.h"
#include "kernel/pit.h"
#include "kernel/memory.h"
#include <libc/stdint.h>
#include <libc/stddef.h>
#include "libc/stdbool.h"
#include "common.h"
#include "menu.h"
#include <screen.h>
#include <multiboot2.h>

// #include "i386/ISR.h"
// #include <kheap.h>
// #include <paging.h>

#define VGA_HEIGHT 25
#define VGA_WIDTH 80
#define VGA_MEMORY (volatile uint16_t*)0xB8000
 

extern uint32_t end; // Linker symbol marking the end of kernel



//uint8_t rainbow_colours[4] = {0x4, 0xE, 0x2, 0x9}; // Rød, gul, grønn, blå

struct multiboot_info {
    uint32_t size; 
    uint32_t reserved; 
    struct multiboot_tag *first;
};


// Skriver til terminalen linje for linje
void write_line_to_terminal(const char* str, int line) {
    if (line >= VGA_HEIGHT) return; // Unngår å skrive utenfor skjermen

    volatile uint16_t* vga = VGA_MEMORY + (VGA_WIDTH * line); // Flytter til riktig linje

    for (int i = 0; str[i] && i < VGA_WIDTH; i++) {
        vga[i] = (rainbow_colours[i % 4] << 8) | str[i]; // Skriver tegn med farge
    }
}



int main(uint32_t magic, uint32_t mb_info_addr)
{

    write_line_to_terminal("Hello", 1);  // Første linje
    write_line_to_terminal("Summernerds!!!", 2);  // Andre linje

 
    // initializing basic systems
    monitor_initialize();
    init_gdt();
    init_idt();
    init_irq();

     register_irq_handler(IRQ1, irq1_keyboard_handler, 0);
     asm volatile("sti");

    // Initializing the kernel memory manager
    init_kernel_memory(&end);

    // call function to activate paging
    init_paging();

    // primt memory layout to screen
    print_memory_layout();

    // Initialize PIT (programmable interval timer)
    init_pit();

    // Here we test the memory allocation
    void *mem1 = malloc(2345);
    void *mem2 = malloc(4321);
    void *mem3 = malloc(3331);

    // print if we get any problem with allocating memory
    printf("Allocated memory blocks at: 0x%x, 0x%x, 0x%x\n", mem1, mem2, mem3);

    // Test PIT sleep
    int counter = 0;

    while (true)
    {
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU)...\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        /*printf("[%d]: Sleeping with interrupts (LOW CPU)...\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);*/
    }
    handle_menu();

    // Usually shouldnt get here, since it then quits kernel main.
    return 0;
}