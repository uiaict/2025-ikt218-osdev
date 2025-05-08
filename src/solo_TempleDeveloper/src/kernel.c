#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/string.h" //string.h includes size_t for us
#include "libc/stdio.h"
#include "idt.h"

#include "gdt.h"
#include "irq.h"



#include <multiboot2.h>

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
    // 4) Enable hardware interrupts
    asm volatile("sti");



    // Trigger my interrupts via assembly
    asm volatile ("int $0x0");  
    asm volatile ("int $0x1");  
    asm volatile ("int $0x2");  

    // Pyramid ASCII Art (7 layers)
    /*printf("\n"
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
        "                ``\"\"YYbbaaaaddPP\"\"''\n");
        printf("\nBooting from multiboot magic: 0x%x\n\n", magic); 
      
    // Comprehensive Format Testing
     printf("=== FORMAT TESTING ===\n");
     printf("Signed: %d | Unsigned: %u | Hex: 0x%x\n", -123456, 123456, 0xABCDEF);
     printf("Char: '%c' | String: \"%s\" | Percent: %%\n", 'T', "TempleOS");
     printf("Edge Cases: Zero:%d | Max:%u | Hex:0x%X\n", 0, 0xFFFFFFFF, 0xCAFEBABE); */
 
    while(true){};
    return 0;
}