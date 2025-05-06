#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h"
#include <multiboot2.h>
#include "libc/gdt.h"
#include "libc/stdlib.h"
#include "libc/stdio.h"
#include "libc/stdarg.h"
#include "libc/idt.h"
#include "libc/interrupts.h"
#include "libc/io.h"
#include "libc/pit.h"
#include "libc/music.h"


#define VIDEO_MEMORY 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25


void print_welcome_message() {
    char* message = "Welcome to My OS!";
    char* video_memory = (char*)0xb8000;  // VGA memory base address
    int i;

    // Write the message to video memory
    for (i = 0; message[i] != '\0'; i++) {
        video_memory[i * 2] = message[i];       // Character
        video_memory[i * 2 + 1] = 0x07;        // Attribute (white text on black background)
    }
}
//typedef struct {
//    uint16_t limit;  // Limit of the IDT
//    uint32_t base;   // Base address of the IDT
//} idt_ptr_t;


// Declare the external pointer to the IDT structure
//extern struct idt_ptr idt_ptr;

extern void idt_load();  // Declare the function from idt.asm
void init_kernel_memory(uint32_t *end);
void init_paging(void);
void print_memory_layout(void);

extern uint32_t end;  // Marks the end of kernel in memory oppgave 4 linje 90 til 110

//void idt_install() {
    // Populate the IDT pointer structure (IDT base and size)
//    uint32_t idt_size = 256 * sizeof(idt_entry_t);  // Size of the IDT (assuming 256 entries)
//    uint32_t idt_base = (uint32_t)&idt;             // Base address of the IDT table (this should point to your IDT array)

    // Set up the IDT pointer (size and base address)
//    idt_ptr.size = idt_size & 0xFFFF;
//    idt_ptr.size |= (idt_size >> 16) & 0xFFFF0000;
//    idt_ptr.base = idt_base;


    // Now call idt_load to load the IDT using the 'lidt' instruction
//    idt_load();
//}

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

// Declare the external function to load the IDT (from idt.asm)
extern void idt_load();  // Declare the function from idt.asm

// Function to initialize and install the IDT
void kernel_main(uint32_t mystruct, uint32_t magic, struct multiboot_info* mb_info_addr) {

    // Test the kernel is running before printing the message
    char* video_memory = (char*)0xb8000;
    video_memory[0] = 'T';
    video_memory[1] = 0x07; // Set color to white
    video_memory[2] = 'E';
    video_memory[3] = 0x07; // Set color to white

    // Now call your print_welcome_message
    print_welcome_message();
      // Delay for 1 second (1000 milliseconds)
    delay(1000);
    
    
    // Install the IDT
    idt_install();

    // Initialize the music system (sound and file system)
    init_music();

    // Play the example WAV file
    play_wav("/assets/music/example.wav");

    // Initialize memory management
    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();

    // Allocate some memory as a test
    int* test = (int*)malloc(sizeof(int));
    *test = 42;
    printf("Allocated integer: %d\n", *test);
    free(test);

     // Initialize the PIT
     init_pit(1000);  // Set PIT to 1000 Hz (1 ms resolution)

     // Sleep using interrupts
     printf("Sleeping for 2 seconds (interrupts)...\n");
     sleep_interrupt(2000);
     printf("Woke up from interrupt sleep!\n");
 
     // Sleep using busy waiting
     printf("Sleeping for 2 seconds (busy waiting)...\n");
     sleep_busy(2000);
     printf("Woke up from busy sleep!\n");

    // Main loop
    while (1) {
        // Main kernel loop (waiting or processing interrupts)
    }
}

//demonstrates eax register with function
int compute(int a, int b) {
    return a + b;
};

typedef struct{
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e[6];

} Mystruct;

static int cursor_pos = 0; //nyeste


void* malloc(size_t size);
void free(void* ptr);

// Overloaded new and delete operators
//void* operator new(size_t size) {
//    return malloc(size);
//}

//void* operator new[](size_t size) {
//    return malloc(size);
//}

//void operator delete(void* ptr) noexcept {
//    free(ptr);
//}

//void operator delete[](void* ptr) noexcept {
//    free(ptr);
//}

// Writes a single character to VGA memory
//int putchar(int c) {
//    static int index = 0;
//    char* video_memory = (char*)0xb8000;

//    if (c == '\n') {
//        index = (index / (VGA_WIDTH * 2) + 1) * VGA_WIDTH * 2;
//    } else {
//        video_memory[index] = c;
//        video_memory[index + 1] = 0x07; // Light gray color
//        index += 2; // Move cursor
//    }

    // Prevent cursor overflow
 //   if (cursor_pos >= VGA_WIDTH * VGA_HEIGHT) {
//        cursor_pos = 0;  // Reset cursor
//    }
//    return c; // Return the written character
//}

// Simple printf implementation (supports only %s for now)
//int printf(const char* format, ...) {
//    va_list args;
//    va_start(args, format);

//    while (*format) {
//        putchar(*format++);
//    }

//    va_end(args);
//    return 0;  // Return success
//}



int main(uint32_t mystruct, uint32_t magic, struct multiboot_info* mb_info_addr) {

    Mystruct* mystructPtr = (Mystruct*)mystruct;             //  Original

    // Verify struct content                                     nytt avsnitt
    uint8_t test_value = mystructPtr->a;  // Should be 33
    if (test_value != 33) 
        return -1;  // Fail-safe check

    char* hello_world = "Hello, World";
    size_t len = strlen(hello_world);
    char* video_memory = (char*)0xb8000;

    //printf("Hello, World!\n");                //implementering av printf
    //printf("The answer is: %d\n", 42);

    // Write to video_memory
    //char* video_memory = (char*)0xb8000;                      original

    //verify struct content
    

    //write hello_world to video memory
    //for (size_t i = 0; i < len; i++) {
    //    video_memory[i * 2] = hello_world[i];
    //    video_memory[i * 2 + 1] = 0x07;
    //}

    // Print a message (if printf is functional in your environment)
    //printf("Hello from printf!\n"); // Test printf
    //printf("The answer is: %d\n", 42); // %d formatting won't work yet, just testing output.


    // Install the IDT (interrupt handling)
    idt_install();

    // Main loop (you can add additional kernel logic here)
    while (1) {
        // Wait here or process interrupts
    }

    //int noop = 0;
    //int res = compute(1, 2);

   
    return 0; // hvis feilkode return 0(lærer har kernel_main())

}