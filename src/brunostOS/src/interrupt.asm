
  
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
    cli                 ; disable interrupt
    push byte %1        ; push interrupt number
    jmp isr_common_stub ; Go to our common handler.
%endmacro 

%macro IRQ 2
  global irq%1
  irq%1:
    cli                 ; disable interrupt
    push byte 0         ; push dummy error
    push byte %2        ; push interrupt number
    jmp irq_common_stub
%endmacro

; define an ASM interrupt handler for each interrupt
; 0-31 used by CPU for exceptions, predetermined by CPU/OS/Arcitechture
; 8, 10-14, 17, 21 push errors, so we use ISR_ERRCODE for those (verify)
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
ISR_ERRCODE 17
ISR 18
ISR 19
ISR 20
ISR_ERRCODE 21
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


; Common ISR handler
[EXTERN isr_handler]    
isr_common_stub:
    pusha             ; Push all the general purpose registers onto the stack to save their state

    mov ax, ds        ; move data segment to ax (first 16bit of eax)
    push eax          ; push eax to stack

    mov ax, 0x10      ; load the kernel data segment descriptor
    mov ds, ax                
    mov es, ax               
    mov fs, ax             
    mov gs, ax               
    push esp          ; push pointer to struct registers 

    call isr_handler  ; Call the ISR handler
    pop eax           ; pop/remove esp from stack

    pop eax           ; restore to previous state
    mov ds, ax               
    mov es, ax             
    mov fs, ax              
    mov gs, ax              

    popa              ; Pops edi,esi,ebp...          
    add esp, 8                
    sti                      
    iret                    
    
; Common IRQ handler
[EXTERN irq_handler]
irq_common_stub:
    pusha             ; Push all the general purpose registers onto the stack to save their state

    mov ax, ds        ; move data segment to ax (first 16bit of eax)
    push eax          ; push eax to stack

    mov ax, 0x10      ; load the kernel data segment descriptor
    mov ds, ax                
    mov es, ax               
    mov fs, ax             
    mov gs, ax   
    push esp          ; push pointer to struct registers    

    call irq_handler  ; Call the IRQ handler
    pop ebx           ; pop/remove esp from stack

    pop ebx           ; restore to previous state
    mov ds, bx               
    mov es, bx             
    mov fs, bx              
    mov gs, bx              

    popa              ; Pops edi,esi,ebp...          
    add esp, 8                
    sti                      
    iret