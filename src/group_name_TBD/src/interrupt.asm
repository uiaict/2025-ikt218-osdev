;CPU wil look up in IDT to find an interuppt handler when it encounter an int. 
;You get no info of whan interuppt caused it when handler is run, so we need a handler for each interrupt,
;Instead of multiple C handlers, we have multiple ASM handlers. All will call a common C handler, but the 
;parameters sent to C will vary depending on which ASM handler was called.

;Some interrupts push an error to stack: if stack isnt uniform, we cant perform dynamically. 
;Stack must be uniform. For those int that does NOT push error, we manually push a dummy error so 
;that the stack is uniform no matter the interrupt.

; CPU interrupt -> look in IDT for the handler for the interrupt it encountered -> isr0-13 runs -> isr_common_stub -> isr_handler()
  
; We want an isr for all interrupts, instead of copypaste, we make a macro  

%macro ISR 1  
  [GLOBAL isr%1]        
  isr%1:
    cli                 ; disable interrupt
    push byte 0         ; push dummy error
    push byte %1        ; push interrupt number
    jmp isr_common_stub ; Go to our common handler.
%endmacro

%macro ISR_ERRCODE 1
  [GLOBAL isr%1]
  isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro 

; define an ASM interrupt handler for each interrupt
; 8, 10-14 push errors, so we use ISR_ERRCODE fo those (verify)
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR_ERRCODE 8
ISR 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31


[EXTERN isr_handler] ; Informs that the C-function isr_handler() exist

isr_common_stub:
; Does stuff
   pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax to stack

   mov ax, ds               ; move data segment to ax (first 16bit of eax)
   push eax                 ; push eax to stack

   mov ax, 0x10  ; load the kernel data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax
; Calls function
   call isr_handler ; Call C-function isr_handler()
; Undoes stuff
   pop eax        ; reload the original data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   popa                     ; Pops edi,esi,ebp...
   add esp, 8     ; Cleans up the pushed error code and pushed ISR number
   sti
   iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP