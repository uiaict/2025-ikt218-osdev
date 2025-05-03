ENTRY(_start)

SECTIONS
{
    /* Load the kernel at 1 MB = 0x100000 */
    . = 1M;

    /* Multiboot header must be in the first 8 KB */
    .boot :
    {
        KEEP(*(.multiboot_header))
    }

    /* Align and include .text section */
    . = ALIGN(4K);
    .text :
    {
        *(.text .text.*)
    }

    /* Read-only data (constants, strings) */
    . = ALIGN(4K);
    .rodata :
    {
        *(.rodata .rodata.*)
    }

    /* Writable data section */
    . = ALIGN(4K);
    .data :
    {
        *(.data .data.*)
    }

    /* Zero-initialized variables */
    . = ALIGN(4K);
    .bss :
    {
        *(.bss .bss.*)
        *(COMMON)
    }

    /* Provide a symbol marking the end of the kernel for heap init */
    . = ALIGN(4K);
    end = .;
    PROVIDE(end = .);
    PROVIDE(_end = .);
    PROVIDE(__end = .);
}
