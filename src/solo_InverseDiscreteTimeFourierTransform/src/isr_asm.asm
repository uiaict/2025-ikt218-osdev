isr_asm.asm:
    GLOBAL idt_load
    EXTERN isr_common
idt_load:
    mov eax, [esp+4]        ; pointer to idt_ptr
    lidt [eax]              ; load the IDT
    ret

; Three ISR stubs that push no error code, their vector number, then 
; jumps to a single C stub (isr_common) that prints and iret.

%macro ISR_NOERR 1
  GLOBAL isr%1
isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3

; IRQ macro
%macro IRQ 1
  GLOBAL irq%1
irq%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15

GLOBAL isr_common_stub
isr_common_stub:
    pusha
    mov ax, ds
    push eax                ; save data segment descriptor
    mov ax, 0x10            ; load kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax


    call isr_common         ; C function: void isr_common(int vector)
    
    pop ebx                 ; restore original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa                    ; Pop edi, esi, ebp etc.
    add esp, 1              ; clean up 1-byte vector
    sti
    iret                    ; pop CS, EIP, EFLAGS, SS, and ESP

