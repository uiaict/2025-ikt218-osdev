; global isr_stub_0
; global isr_stub_1
; global isr_stub_2
; global isr_stub_3
; global isr_stub_4
; global isr_stub_5
; global isr_stub_6
; global isr_stub_7
; global isr_stub_8
; global isr_stub_9
; global isr_stub_10
; global isr_stub_11
; global isr_stub_12
; global isr_stub_13
; global isr_stub_14
; global isr_stub_15
; global isr_stub_16
; global isr_stub_17
; global isr_stub_18
; global isr_stub_19
; global isr_stub_20
; global isr_stub_21
; global isr_stub_22
; global isr_stub_23
; global isr_stub_24
; global isr_stub_25
; global isr_stub_26
; global isr_stub_27
; global isr_stub_28
; global isr_stub_29
; global isr_stub_30
; global isr_stub_31

%macro isr_no_err_stub 1
global isr%1
isr%1
    push byte 0
    push %1
    jmp isr_common_stub
%endmacro

%macro isr_err_stub 1
global isr%1
isr%1
    push %1
    jmp isr_common_stub
%endmacro

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

extern int_handler
isr_common_stub:
    pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds               ; Lower 16-bits of eax = ds.
    push eax                 ; save the data segment descriptor

    mov ax, 0x10             ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Extract the interrupt number from the stack
    ; (32 bytes for pusha + 4 bytes for ds push)
    mov eax, [esp+36]        ; Get the interrupt number
    push eax                 ; Pass it as parameter to C function
    call int_handler         ; Call our C handler
    add esp, 4               ; Clean up the parameter we pushed

    pop ebx                  ; reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa                     ; Pops edi,esi,ebp...
    add esp, 8               ; Cleans up the pushed error code and pushed ISR number
    sti                      ; Re-enable interrupts before returning
    iret                     ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP