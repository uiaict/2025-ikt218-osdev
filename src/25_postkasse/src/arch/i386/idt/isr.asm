; Macros for generating ISR stubs for exceptions that push an error code
%macro isr_err_stub 1
global isr_stub_%+%1:           ;Make isr_stub_<n> visible to C code
isr_stub_%+%1:                  ;Define the label where the CPU will jump when the interrupt occurs
    call exception_handler      ;Call the exception handler from idt.c
    ;push dword %1              ;pass interrupt number as parameter for isr_common_handler
    ;call isr_common_handler    ;Print the interupt number function
    ;add esp, 4                 ;clean up the stack after call
    iret                        ;Return from the interrupt
%endmacro

; Macros for generating ISR stubs for exceptions that DO NOT push an error code
%macro isr_no_err_stub 1
global isr_stub_%+%1:           ;Make isr_stub_<n> visible to C code
isr_stub_%+%1:                  ;Define the label where the CPU will jump when the interrupt occurs
    call exception_handler      ;Call the exception handler from idt.c
    ;push dword %1              ;pass interrupt number as parameter for isr_common_handler
    ;call isr_common_handler    ;Print the interupt number function
    ;add esp, 4                 ;clean up the stack after call
    iret                        ;Return from the interrupt
%endmacro


extern exception_handler        ;Decleare the exception_handler function from idt.c

isr_no_err_stub 0               ; ISR for interrupt 0
isr_no_err_stub 1               ; ISR for interrupt 1
isr_no_err_stub 2               ; ISR for interrupt 2
