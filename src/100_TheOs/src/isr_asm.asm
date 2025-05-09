;
; interrupt.s -- Contains interrupt service routine wrappers.
;                Based on Bran's kernel development tutorials.
;                Rewritten for JamesM's kernel development tutorials.

; This macro creates a stub for an ISR which does not pass
; an error code. The first parameter is the interrupt number.
%macro ISR_NO_ERROR 1
  global isr%1
  isr%1:
    ;cli                        ; Disable interrupts.
    push byte 0                 ; Push 0 for the error code.
    push  %1                    ; Push the interrupt number
    jmp isr_common_stub         ; Jump to the common ISR stub.
%endmacro

; This macro creates a stub for an ISR which does pass
; an error code. The first parameter is the interrupt number.
%macro ISR_ERROR 1
  global isr%1
  isr%1:
    ;cli                         ; Disable interrupts.
    push %1                      ; Push the error code.
    jmp isr_common_stub          ; Jump to the common ISR stub.
%endmacro

; This macro creates a stub for an IRQ that remaps the
; interrupt number to the correct IRQ number. The first
; parameter is the interrupt number, and the second is the
; IRQ number.
%macro IRQ 2
  global irq%1
  irq%1:
    ;cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

; This is the list of ISRs. The first column is the
; interrupt number, the second column is the name of
; the ISR. The third column is the error code.
ISR_NO_ERROR 0
ISR_NO_ERROR 1
ISR_NO_ERROR 2
ISR_NO_ERROR 3
ISR_NO_ERROR 4
ISR_NO_ERROR 5
ISR_NO_ERROR 6
ISR_NO_ERROR 7
ISR_ERROR   8
ISR_NO_ERROR 9
ISR_ERROR   10
ISR_ERROR   11
ISR_ERROR   12
ISR_ERROR   13
ISR_ERROR   14
ISR_NO_ERROR 15
ISR_NO_ERROR 16
ISR_ERROR 17
ISR_NO_ERROR 18
ISR_NO_ERROR 19
ISR_NO_ERROR 20
ISR_ERROR 21
ISR_NO_ERROR 22
ISR_NO_ERROR 23
ISR_NO_ERROR 24
ISR_NO_ERROR 25
ISR_NO_ERROR 26
ISR_NO_ERROR 27
ISR_NO_ERROR 28
ISR_NO_ERROR 29
ISR_NO_ERROR 30
ISR_NO_ERROR 31
ISR_NO_ERROR 128
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

; isr controller in isr.c
extern isr_controller

; This is our common ISR stub. It saves the processor state,
; sets up for kernel mode segments, calls the C-level fault
; controller, and finally restores the stack frame.
isr_common_stub:
    pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds               ; Lower 16-bits of eax = ds.
    push eax                 ; save the data segment descriptor

    mov ax, 0x10             ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp                 ; CRITICAL FIX: Pass pointer to registers structure
    call isr_controller      ; Call the C handler
    add esp, 4               ; Clean up the pushed parameter

    pop ebx                  ; reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa                     ; Pops edi,esi,ebp...
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    sti
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
; isr controller in isr.c
extern irq_controller


; This is our common ISR stub. It saves the processor state,
; sets up for kernel mode segments, calls the C-level fault
; controller, and finally restores the stack frame.
irq_common_stub:
    pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds               ; Lower 16-bits of eax = ds.
    push eax                 ; save the data segment descriptor

    mov ax, 0x10             ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp                 ; CRITICAL FIX: Pass pointer to registers structure
    call irq_controller      ; Call the C handler
    add esp, 4               ; Clean up the pushed parameter

    pop ebx                  ; reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa                     ; Pops edi,esi,ebp...
    add esp, 8               ; Cleans up the pushed error code and pushed ISR number
    iret                     ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP


