; src/irq_stubs.asm

extern irq_handler   ; Import the C function for interrupt dispatching

global irq0_stub
global irq1_stub
global irq2_stub
global irq3_stub
global irq4_stub
global irq5_stub
global irq6_stub
global irq7_stub
global irq8_stub
global irq9_stub
global irq10_stub
global irq11_stub
global irq12_stub
global irq13_stub
global irq14_stub
global irq15_stub

section .text

irq0_stub:
    pusha                  ; Save all general-purpose registers
    cld                    ; Clear direction flag
    push 0                 ; Push IRQ number (0 for IRQ0)
    call irq_handler       ; Call the C handler
    add esp, 4             ; Clean up the stack (remove IRQ number)
    mov al, 0x20           ; EOI command
    out 0x20, al           ; Send EOI to master PIC
    popa                   ; Restore all registers
    iret                   ; Return from interrupt

irq1_stub:
    pusha                  ; Save all general-purpose registers
    cld                    ; Clear direction flag
    push 1                 ; Push IRQ number (1 for IRQ1)
    call irq_handler       ; Call the C handler
    add esp, 4             ; Clean up the stack (remove IRQ number)
    mov al, 0x20           ; EOI command
    out 0x20, al           ; Send EOI to master PIC
    popa                   ; Restore all registers
    iret                   ; Return from interrupt

irq2_stub:    iret
irq3_stub:    iret
irq4_stub:    iret
irq5_stub:    iret
irq6_stub:    iret
irq7_stub:    iret
irq8_stub:    iret
irq9_stub:    iret
irq10_stub:   iret
irq11_stub:   iret
irq12_stub:   iret
irq13_stub:   iret
irq14_stub:   iret
irq15_stub:   iret