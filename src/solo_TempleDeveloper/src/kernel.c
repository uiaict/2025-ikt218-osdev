#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h" //string.h includes size_t for us
#include "libc/stdio.h"
#include "pit.h"

#include <multiboot2.h>

//GDT, IDT and interrupts-related
#include "idt.h"
#include "gdt.h"
#include "irq.h"

//memory, paging and PIT
#include "memory.h"
#include "pit.h"

extern uint32_t end;  //This is defined in the linker.ld
extern int kernel_main(void);  //cpp++ for overloading

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    //Set up the GDT
    init_gdt();

    //set up the IDT and IQR with logger
    install_idt();
    irq_install();
    asm volatile("sti");   // Enable hardware interrupts

    // Initialize the kernel's memory manager using the end address of the kernel.
    init_kernel_memory(&end); 

    // Initialize paging for memory management.
    init_paging(); 

    //Print memory information.
    //print_memory_layout(); 

    // Trigger my interrupts via assembly
    asm volatile ("int $0x0");  

    //test allocatin some memory
    void* some_memory = malloc(12345); 
    void* memory2 = malloc(54321); 

    //set up the PIT
    init_pit();

    //ASCII Art because I can (no other reson needed)
   printf(
        "                .,aadd\"'    `\"bbaa,.\n"
        "            ,ad8888P'          `Y8888ba,\n"
        "         ,a88888888              88888888a,\n"
        "       a88888888888              88888888888a\n"
        "     a8888888888888b,          ,d8888888888888a\n"
        "    d8888888888888888b,_    _,d8888888888888888b\n"
        "   d88888888888888888888888888888888888888888888b\n"
        "  d8888888888888888888888888888888888888888888888b\n"
        " I888888888888888888888888888888888888888888888888I\n"
        ",88888888888888888888888888888888888888888888888888,\n"
        "I8888888888888888PY8888888PY88888888888888888888888I\n"
        "8888888888888888\"  \"88888\"  \"88888888888888888888888\n"
        "8::::::::::::::'    `:::'    `:::::::::::::::::::::8\n"
        "Ib:::::::::::\"        \"        `::::::' `:::::::::dI\n"
        "`8888888888P            Y88888888888P     Y88888888'\n"
        " Ib:::::::'              `:::::::::'       `:::::dI\n"
        "  Yb::::\"                  \":::::\"           \"::dP\n"
        "   Y88P                      Y8P               `P\n"
        "    Y'                        \"\n"
        "                                `:::::::::::;8\"\n"
        "       \"888888888888888888888888888888888888\"\n"
        "         `\"8;::::::::::::::::::::::::::;8\"'\n"
        "            `\"Ya;::::::::::::::::::;aP\"'\n"
        "                ``\"\"YYbbaaaaddPP\"\"''");
        printf("\nTemple Developer\nBooting from multiboot magic: 0x%x", magic);
        printf("  Moving on in 5 seconds!");
        sleep_busy(5000); 
      
    // Comprehensive Format Testing
     printf("=== FORMAT TESTING ===\n");
     printf("Signed: %d | Unsigned: %u | Hex: 0x%x\n", -123456, 123456, 0xABCDEF);
     printf("Char: '%c' | String: \"%s\" | Percent: %%\n", 'T', "TempleOS");
     printf("Edge Cases: Zero:%d | Max:%u | Hex:0x%X\n", 0, 0xFFFFFFFF, 0xCAFEBABE); 


     //call the cpp kernel
    return kernel_main();

    //just in case so we don't crash
    while(true) {}
}