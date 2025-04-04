global irq0_stub
global irq_common_stub

extern irq_handler

%macro IRQ 2
  global irq%1
  irq%1:
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

irq_common_stub:
    ; Push all general-purpose registers
    pusha

    ; Save the current data segment selector (ds) 
    mov ax, ds
    push eax

    ; Load the kernel data segment selector (assumes 0x10 is valid for your kernel)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; The IRQ number is at [esp+36] 
    ; (32 bytes for pusha + 4 bytes for ds push)
    mov eax, [esp+36]
    push eax                ; Pass IRQ number as parameter to C function
    call irq_handler
    add esp, 4              ; Clean up the parameter

    ; Restore the original data segment selector
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 8              ; Clean up error code and IRQ number
    sti                     ; Make sure interrupts are enabled when we return
    iret

; Define all IRQ stubs
IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47