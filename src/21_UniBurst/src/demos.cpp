extern "C" {
    #include "libc/stdint.h"
    #include "libc/stddef.h"
    #include "libc/stdio.h"
    #include "libc/stdbool.h"
    #include "libc/stdlib.h"
    #include "libc/string.h"
    #include "pit.h"
    #include "io.h"
    #include "macros.h"
}

#include "demos.h"
#include "song.h"



// triggers interrupts
void isrDemo() {
    asm volatile("int $0x00");
    asm volatile("int $0x01");
    asm volatile("int $0x02");
    asm volatile("int $0x03");
    asm volatile("int $0x04");
    asm volatile("int $0x05");
    asm volatile("int $0x06");
    asm volatile("int $0x07");
    asm volatile("int $0x08");
    asm volatile("int $0x09");
    asm volatile("int $0x0A");
    asm volatile("int $0x0B");
    asm volatile("int $0x0C");
    asm volatile("int $0x0D");
    printf("Skipping page fault as this causes panic\n");
    asm volatile("int $0x0F");
    asm volatile("int $0x10");
    asm volatile("int $0x11");
    asm volatile("int $0x12");
    asm volatile("int $0x13");
    asm volatile("int $0x14");
    asm volatile("int $0x15");
    asm volatile("int $0x16");
    printf("Interrupts are working!\n");

}

// causes page fault
void pageFaultDemo() {
    printf("Running page fault demo will cause a panic. Continue? [y/n]\n");
    char c = getchar();
    videoMemory[cursorPos] = ' ';
    cursorPos -= 2;
    // runs or aborts
    if (c == 'y'){
        uint32_t *ptr = (uint32_t*)0xE0000000;
        uint32_t do_page_fault = *ptr;
    } else {
        printf("Aborted\n");
    }
}


// shows printf function
void printDemo() {

    char strTest[] = "Hello World!";                
    int intTest = 123;
    unsigned int uintTest = 1234567890;
    float floatTest = 3.14;
    double doubleTest = 3.14159;
    char hexTest[] = "0x01";

    printf("Printing a string: %s\n", strTest);     
    printf("Printing an integer: %d\n", intTest);
    printf("Printing an unsigned integer: %u\n", uintTest);
    printf("Printing a float: %f\n", floatTest);
    printf("Printing a float with .1f precition: %.1f\n", floatTest);
    printf("Printing a double: %f\n", doubleTest);
    printf("Printing a hex: %s\n", hexTest);    
}

// shows sleep function using pit
void pitDemo() {
    printf("Using interrupt to sleep in ten intervals of 1sec\n");
    for (int i = 1; i < 10; i++) {
        printf("%dsec\n", i);
        sleepInterrupt(1000);
    }
    printf("i just slept!\n");
}
