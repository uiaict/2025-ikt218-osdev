OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
 
ENTRY(_start)
 
SECTIONS
{
    /* Place the multiboot header first */
    . = 1M;
 
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot_header)
        *(.text)
    }
 
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }
 
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }
 
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}