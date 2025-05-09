#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdio.h>
#include <kernel/memory/memory.h>
#include <ui/menu.h>
#include <ui/memory_viewer.h>
#include <ui/music_player.h>
#include <ui/ui_common.h>
#include <driver/include/keyboard.h>
#include <driver/include/port_io.h>

////////////////////////////////////////
// External Kernel Setup Functions
////////////////////////////////////////
extern void gdt_install(void);
extern void idt_install(void);
extern void init_irq(void);
extern void init_paging(void);
extern void keyboard_initialize(void);
extern void terminal_initialize(void);
extern void init_pit(void);
extern void print_memory_layout(void);
extern void init_kernel_memory(uint32_t* kernel_end);
extern void* malloc(size_t size);

////////////////////////////////////////
// Keyboard Buffer Utilities
////////////////////////////////////////
extern bool keyboard_buffer_empty(void);
extern uint8_t keyboard_buffer_dequeue(void);
extern void clear_keyboard_buffer(void);

////////////////////////////////////////
// Kernel End Symbol from Linker
////////////////////////////////////////
extern uint32_t end;

////////////////////////////////////////
// Global Menu
////////////////////////////////////////
Menu main_menu;

////////////////////////////////////////
// Menu Action Prototypes
////////////////////////////////////////
void show_system_info(void);
void test_memory_allocations(void);
void system_halt(void);

////////////////////////////////////////
// Menu Action: Show System Info
////////////////////////////////////////
void show_system_info(void) {
    clear_screen();

    terminal_setcursor(26, 1);
    terminal_write("SYSTEM INFORMATION");

    terminal_setcursor(5, 3);
    terminal_write("UiA Operating System - Assignment 6");

    terminal_setcursor(5, 5);
    terminal_write("CPU: x86 (32-bit protected mode)");

    print_memory_layout();

    terminal_setcursor(5, 12);
    terminal_write("Features:");
    terminal_setcursor(5, 13);
    terminal_write("- Memory Management (malloc/free)");
    terminal_setcursor(5, 14);
    terminal_write("- Paging (identity mapping)");
    terminal_setcursor(5, 15);
    terminal_write("- Programmable Interval Timer (PIT)");
    terminal_setcursor(5, 16);
    terminal_write("- PC Speaker driver");
    terminal_setcursor(5, 17);
    terminal_write("- Interactive Menu System");

    terminal_setcursor(5, 19);
    terminal_write("Created by: [Sinder Winæs]");
    terminal_setcursor(5, 20);
    terminal_write("University of Agder - IKT218 - 2025");

    terminal_setcursor(24, 22);
    terminal_write("Press any key to return to menu...");

    keyboard_get_key();
    clear_screen();
}

////////////////////////////////////////
// Menu Action: Test Memory Allocations
////////////////////////////////////////
void test_memory_allocations(void) {
    clear_screen();

    terminal_setcursor(26, 1);
    terminal_write("MEMORY ALLOCATION TEST");

    terminal_setcursor(5, 3);
    terminal_write("Allocating memory blocks...");

    void* block1 = malloc(8192);
    void* block2 = malloc(16384);
    void* block3 = malloc(4096);
    void* block4 = malloc(32768);

    terminal_setcursor(5, 5);
    printf("Block 1: %d bytes at 0x%08X", 8192, (uint32_t)block1);

    terminal_setcursor(5, 6);
    printf("Block 2: %d bytes at 0x%08X", 16384, (uint32_t)block2);

    terminal_setcursor(5, 7);
    printf("Block 3: %d bytes at 0x%08X", 4096, (uint32_t)block3);

    terminal_setcursor(5, 8);
    printf("Block 4: %d bytes at 0x%08X", 32768, (uint32_t)block4);

    terminal_setcursor(5, 10);
    terminal_write("Memory allocation test complete!");

    terminal_setcursor(5, 12);
    print_memory_layout();

    terminal_setcursor(5, 18);
    terminal_write("Press any key to return to menu...");

    keyboard_get_key();
    clear_screen();
}

////////////////////////////////////////
// Menu Action: Halt the System
////////////////////////////////////////
void system_halt(void) {
    clear_screen();

    terminal_setcursor(26, 10);
    terminal_write("SYSTEM HALTED");

    terminal_setcursor(14, 12);
    terminal_write("Your OS is taking a well-deserved rest. Goodbye!");

    outw(0x604, 0x2000);

    for(;;) {
        asm volatile("hlt");
    }
}


////////////////////////////////////////
// Kernel Main Entry Point
////////////////////////////////////////
int kernel_main_c(uint32_t magic, void* mb_info_addr) {
    terminal_initialize();
    gdt_install();
    idt_install();
    init_irq();
    init_kernel_memory(&end);
    init_paging();
    keyboard_initialize();
    init_pit();

    asm volatile("sti");  // Enable interrupts globally

    terminal_setcursor(18, 8);
    terminal_write("UiA Operating System - Assignment 6");
    terminal_setcursor(25, 10);
    terminal_write("Interactive Menu System");
    terminal_setcursor(24, 12);
    terminal_write("Press any key to continue...");

    keyboard_get_key();
    clear_keyboard_buffer();  // Clear any buffered input before starting menu

    // Pre-allocate memory blocks to populate memory viewer
    void* block1 = malloc(12345);
    void* block2 = malloc(54321);
    void* block3 = malloc(7890);

    // Initialize menu system
    menu_init(&main_menu, "UiA OS - Main Menu");

    // Add menu items and associated callbacks
    menu_add_item(&main_menu, "Memory Viewer", launch_memory_viewer);
    menu_add_item(&main_menu, "Music Player", launch_music_player);
    menu_add_item(&main_menu, "System Information", show_system_info);
    menu_add_item(&main_menu, "Test Memory Allocations", test_memory_allocations);
    menu_add_item(&main_menu, "Halt System", system_halt);

    // Run main menu loop
    main_menu.running = true;
    while (main_menu.running) {
        menu_render(&main_menu);
        KeyCode key = keyboard_get_key();

        if (key == KEY_ESC) {
            // Special ESC key behavior for exiting from main menu
            system_halt();
        } else {
            menu_handle_input(&main_menu, key);
        }
    }

    system_halt();  // Fallback halt
    return 0;
}