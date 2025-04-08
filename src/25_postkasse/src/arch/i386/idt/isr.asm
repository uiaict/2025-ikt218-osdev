; Macros for generating ISR stubs for exceptions that push an error code
%macro isr_err_stub 1
global isr_stub_%+%1:           ;Make isr_stub_<n> visible to C code
isr_stub_%+%1:                  ;Define the label where the CPU will jump when the interrupt occurs
    cli
    push dword %1               ;pass interrupt number as parameter for isr_common_handler
    call isr_common_stub     ;Print the interupt number function
    add esp, 4                  ;clean up the stack after call
    iret                        ;Return from the interrupt
%endmacro

; Macros for generating ISR stubs for exceptions that DO NOT push an error code
%macro isr_no_err_stub 1
global isr_stub_%+%1:           ;Make isr_stub_<n> visible to C code
isr_stub_%+%1:                  ;Define the label where the CPU will jump when the interrupt occurs
    cli
    push dword %1               ;pass interrupt number as parameter for isr_common_handler
    call isr_common_stub     ;Print the interupt number function
    add esp, 4                  ;clean up the stack after call
    iret                        ;Return from the interrupt
%endmacro

; Macro that generates a stub for an IRQ
; Two parameters are passed to the macro
; 1. The IRQ number
; 2. ISR number it is remapped to
%macro IRQ 2
  global irq%1
  irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

extern exception_handler        ;Decleare the exception_handler function from idt.c

; Create stubs
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

; Create the IRQ stubs
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


;Declare the function from isr.c as external
extern isr_handler
isr_common_stub:
    pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds               ; Lower 16-bits of eax = ds.
    push eax                 ; save the data segment descriptor

    mov ax, 0x10  ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler

    pop ebx        ; reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa                     ; Pops edi,esi,ebp...
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    sti
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP


;Declare the function from isr.c as external
extern irq_handler
; Common stub for all IRQ's
; saves processor state, sets up for the kernel mode segments,
; calls the c-level fault handler and restores the stack
irq_common_stub:
   pusha                    ;Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov ax, ds               ;Lower 16-bits of eax = ds.
   push eax                 ; save the data segment descriptor

   mov ax, 0x10             ; load the kernel data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   call irq_handler

   pop ebx                  ; reload the original data segment descriptor
   mov ds, bx
   mov es, bx
   mov fs, bx
   mov gs, bx

   popa                     ; Pops edi,esi,ebp...
   add esp, 8               ; Cleans up the pushed error code and pushed ISR number
   sti
   iret                     ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP



; Make the function global and create the isr_stub_table
global isr_stub_table
isr_stub_table:
%assign i 0 
%rep    32 
    dd isr_stub_%+i
%assign i i+1 
%endrep