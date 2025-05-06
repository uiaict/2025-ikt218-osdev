extern isr_handler

%macro ISR_NOERR 1
global isr%1
isr%1:
    cli
    push dword 0         ; Dummy error code
    push dword %1        ; Interrupt number
    jmp isr_common_stub
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    cli
    push dword %1        ; Interrupt number (real error code on stack)
    jmp isr_common_stub
%endmacro

section .text
isr_common_stub:
    pusha                 ; Push registers in this order:
                          ; edi, esi, ebp, esp, ebx, edx, ecx, eax

    push dword [esp + 32] ; Push original esp before pusha (so it's preserved)
    push dword [esp + 32] ; Push original ebp
    push dword [esp + 32] ; Push original esi
    push dword [esp + 32] ; Push original edi

    ; Push pointer to struct (esp points to eax)
    mov eax, esp
    push eax
    call isr_handler
    add esp, 4

    ; Remove manually-pushed edi/esi/ebp/esp
    add esp, 16

    popa
    add esp, 8
    sti
    iret



; ISRs 0–31
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR  10
ISR_ERR  11
ISR_ERR  12
ISR_ERR  13
ISR_ERR  14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR  17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31