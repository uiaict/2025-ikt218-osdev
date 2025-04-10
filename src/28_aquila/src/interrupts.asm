%macro ISR_NOERRCODE 1
  global isr%1
  isr%1:
    push byte 0         
    push %1             
    jmp isr_common_stub
%endmacro

%macro IRQ 2
  global irq%1
  irq%1:
    push byte 0         
    push byte %2        
    jmp irq_common_stub
%endmacro

extern isr_handler
extern irq_handler

ISR_NOERRCODE 0        
ISR_NOERRCODE 1       
ISR_NOERRCODE 14       

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

isr_common_stub:
    cli
    pusha                  
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10           
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp               
    call isr_handler
    add esp, 4             

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8             
    sti
    iret

irq_common_stub:
    cli
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp               
    call irq_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8             
    sti
    iret
