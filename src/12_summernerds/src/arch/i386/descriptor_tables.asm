; Descriptor Tables assembly file
;This file contains the assembly code for the Global Descriptor Table (GDT) and Interrupt Descriptor Table (IDT) management.


[GLOBAL gdt_flush]    ; Allows the C code to call gdt_flus.

gdt_flush:
    mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
    lgdt [eax]        ; Load the new GDT pointer

    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret

[GLOBAL idt_flush]    ; Allows the C code to call idt_flush.

<<<<<<< HEAD:src/12_summernerds/src/arch/i386/gdt.asm
=======
idt_flush:
   mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter.
   lidt [eax]        ; Load the IDT pointer.
   ret

>>>>>>> 88dd27d724e8ebadce9fa19d22b22ef2489674e7:src/12_summernerds/src/arch/i386/descriptor_tables.asm
