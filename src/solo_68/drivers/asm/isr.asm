[bits 32]

global idt_load
extern isr_handler
extern irq_handler

idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret

; ------------------- ISR Table (0x00–0x1F) -------------------
section .text
global isr_stub_table
isr_stub_table:
%assign i 0
%rep 32
    extern isr_stub_ %+ i
    dd isr_stub_ %+ i
%assign i i+1
%endrep

%assign i 0
%rep 32
isr_stub_ %+ i:
    cli
    push dword i
    call isr_handler
    add esp, 4
    sti
    iret
%assign i i+1
%endrep

; ------------------- IRQ Table (0x20–0x2F) -------------------
global irq_stub_table
irq_stub_table:
%assign i 0
%rep 16
    extern irq_stub_ %+ i
    dd irq_stub_ %+ i
%assign i i+1
%endrep

%assign i 0
%rep 16
irq_stub_ %+ i:
    cli
    push dword 32 + i
    call irq_handler
    add esp, 4
    sti
    iret
%assign i i+1
%endrep
