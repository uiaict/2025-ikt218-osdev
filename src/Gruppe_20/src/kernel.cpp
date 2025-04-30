#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include "libc/system.h"

#include <multiboot2.h>
#include "libc/gdt.h"
#include "libc/idt.h"
#include "pit.h"
#include "interrupts.h"
#include "memory/memory.h"


#include "libc/stdlib.h" // for malloc and free
#include "libc/stddef.h" // for size_t


void* operator new(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) panic("Memory allocation failed");
    return ptr;
}

void* operator new[](size_t size) {
    void* ptr = malloc(size);
    if (!ptr) panic("Memory allocation failed");
    return ptr;
}

void operator delete(void* ptr) noexcept { free(ptr); }
void operator delete[](void* ptr) noexcept { free(ptr); }
void operator delete(void* ptr, size_t) noexcept { free(ptr); }
void operator delete[](void* ptr, size_t) noexcept { free(ptr); }


// Kernel entry point
extern "C" int kernel_main() {
    // Initialize core systems
    init_gdt();
    init_idt();
    init_pit();
    
    // Set up interrupt handlers
    register_interrupt_handler(3, [](registers_t* regs, void* context) {
        printf("Breakpoint hit!\n");
    }, nullptr);

    register_interrupt_handler(14, [](registers_t* regs, void* context) {
        uint32_t fault_addr;
        asm volatile("mov %%cr2, %0" : "=r" (fault_addr));
        printf("Page fault at 0x%x (", fault_addr);
        if (regs->err_code & 0x1) printf("protection violation ");
        if (regs->err_code & 0x2) printf("write attempt ");
        if (regs->err_code & 0x4) printf("user-mode ");
        printf(")\n");
        panic("Page fault");
    }, nullptr);

    // Enable interrupts
    asm volatile("sti");

    printf("Kernel initialized successfully\n");

    // Main kernel loop
    while(true) {
        asm volatile("hlt"); // Save power when idle
    }
    return 0;
}

// Multiboot entry point
extern "C" __attribute__((noreturn)) void kmain(uint32_t magic, uint32_t* mb_info) {
    // Initialize memory management first
    init_kernel_memory(mb_info);
    init_paging();

    // Hand off to kernel proper
    kernel_main();
    // Should never return
    for(;;) asm volatile("hlt");
}