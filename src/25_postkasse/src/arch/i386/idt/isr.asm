%macro isr_err_stub 1
global isr_stub_%+%1:
isr_stub_%+%1:
    call exception_handler
    ;push dword %1                ;pass interrupt number to C
    ;call isr_common_handler
    ;add esp, 4                  ;clean up the stack after call
    iret 
%endmacro

%macro isr_no_err_stub 1
global isr_stub_%+%1:
isr_stub_%+%1:
    call exception_handler
    ;push dword %1                ;pass interrupt number to C
    ;call isr_common_handler
    ;add esp, 4                  ; clean up the stack after call
    iret
%endmacro


extern exception_handler
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
