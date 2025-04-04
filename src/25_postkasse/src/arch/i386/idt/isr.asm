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

; Make the function global and create the isr_stub_table
global isr_stub_table
isr_stub_table:
%assign i 0 
%rep    32 
    dd isr_stub_%+i
%assign i i+1 
%endrep