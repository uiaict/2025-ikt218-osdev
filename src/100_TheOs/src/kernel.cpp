// kernel.cpp
extern "C" {
    #include "libc/system.h"
    #include "memory/memory.h"
    #include "common.h"
    #include "interrupts.h"
    #include "song/song.h"
    #include "monitor.h"
    #include "pit.h"
    
    void panic(const char* reason);
    void init_gdt(void);
    void start_idt(void);
    void init_irq(void);
    void start_isr_controllers(void);
    void start_keyboard(void);
    void display_prompt(void);
    void init_pit(void);
    char scancode_to_ascii(uint8_t scancode);
    int printf(const char* fmt, ...);
    // Add these declarations if they're defined in your C code
    void keyboard_handler(void);
    void load_interrupt_controller(uint8_t n, isr_t controller, void*);
}

// Memory operators with proper size_t definition
void* operator new(unsigned long size) { return malloc(size); }
void* operator new[](unsigned long size) { return malloc(size); }
void operator delete(void* ptr) noexcept { free(ptr); }
void operator delete[](void* ptr) noexcept { free(ptr); }
void operator delete(void* ptr, unsigned long) noexcept { free(ptr); }
void operator delete[](void* ptr, unsigned long) noexcept { free(ptr); }

extern "C" int kernel_main(void) {
    printf("Booting C++ kernel...\n");
    
    // 1. Initialize GDT
    init_gdt();
    printf("GDT initialized\n");
    
    // 2. Initialize IDT
    start_idt();
    printf("IDT initialized\n");
    
    // 3. Initialize IRQ system
    start_irq();
    printf("IRQ system initialized\n");
    

    // 4. Initialize ISR handlers
    start_isr_controllers();
    printf("ISR handlers initialized\n");

    // 5. Initialize PIT
    init_pit();

    // 6. Initialize keyboard
    printf("Starting keyboard initialization...\n");
    start_keyboard();
    printf("Keyboard initialized\n");
    
    // 7. Enable interrupts globally
    asm volatile("sti");
    printf("Interrupts enabled\n");


    // Prompt
    printf("Ready. Type something below:\n");
    display_prompt();
    
    // Main loop
    while (true) {
        asm volatile("hlt");
    }
    
    return 0;
}