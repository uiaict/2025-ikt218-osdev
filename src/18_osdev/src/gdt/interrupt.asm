; adopted from jamesM's kernel development tutorials the GDT and IDT: https://archive.is/L3pyA

; macros for no error code, errorcode and irq
%macro ISR_NOERRCODE 1  
  [GLOBAL isr%1]        ; %1 accesses the first parameter.
  isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
  [GLOBAL isr%1]
  isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro

%macro IRQ 2
  global irq%1
  irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

; ISRs 0 - 31 and what they are for
ISR_NOERRCODE 0    ; Divide-by-zero
ISR_NOERRCODE 1    ; Debug
ISR_NOERRCODE 2    ; Non-maskable interrupt
ISR_NOERRCODE 3    ; Breakpoint
ISR_NOERRCODE 4    ; Overflow
ISR_NOERRCODE 5    ; Bound Range Exceeded
ISR_NOERRCODE 6    ; Invalid Opcode
ISR_NOERRCODE 7    ; Device Not Available
ISR_ERRCODE   8    ; Double Fault
ISR_NOERRCODE 9    ; Coprocessor Segment Overrun (reserved)
ISR_ERRCODE   10   ; Invalid TSS
ISR_ERRCODE   11   ; Segment Not Present
ISR_ERRCODE   12   ; Stack-Segment Fault
ISR_ERRCODE   13   ; General Protection Fault
ISR_ERRCODE   14   ; Page Fault
ISR_NOERRCODE 15   ; Reserved
ISR_NOERRCODE 16   ; x87 Floating-Point Exception
ISR_ERRCODE   17   ; Alignment Check
ISR_NOERRCODE 18   ; Machine Check
ISR_NOERRCODE 19   ; SIMD Floating-Point Exception
ISR_NOERRCODE 20   ; Virtualization Exception
ISR_NOERRCODE 21   ; Control Protection Exception
ISR_NOERRCODE 22   ; Reserved
ISR_NOERRCODE 23   ; Reserved
ISR_NOERRCODE 24   ; Reserved
ISR_NOERRCODE 25   ; Reserved
ISR_NOERRCODE 26   ; Reserved
ISR_NOERRCODE 27   ; Reserved
ISR_NOERRCODE 28   ; Hypervisor Injection Exception
ISR_NOERRCODE 29   ; VMM Communication Exception
ISR_NOERRCODE 30   ; Security Exception
ISR_NOERRCODE 31   ; Reserved

; IRQs:
IRQ   0,    32   ; Timer 
IRQ   1,    33   ; Keyboard 
IRQ   2,    34   ; Cascade 
IRQ   3,    35   ; COM2 
IRQ   4,    36   ; COM1 
IRQ   5,    37   ; LPT2 
IRQ   6,    38   ; Floppy Disk 
IRQ   7,    39   ; LPT1 
IRQ   8,    40   ; Real-Time 
IRQ   9,    41   ; Available 
IRQ  10,    42   ; Available 
IRQ  11,    43   ; Available 
IRQ  12,    44   ; PS/2 Mouse
IRQ  13,    45   ; FPU 
IRQ  14,    46   ; Primary IDE 
IRQ  15,    47   ; Secondary IDE 

; isr/irq handler functions are called in the common stub
[EXTERN isr_handler]
[EXTERN irq_handler]

; common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
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

  pop eax        ; reload the original data segment descriptor
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  popa                     ; Pops edi,esi,ebp...
  add esp, 8     ; Cleans up the pushed error code and pushed ISR number
  sti
  iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

; common IRQ stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
irq_common_stub:
  pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

  mov ax, ds               ; Lower 16-bits of eax = ds.
  push eax                 ; save the data segment descriptor

  mov ax, 0x10  ; load the kernel data segment descriptor
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  call irq_handler

  pop ebx        ; reload the original data segment descriptor
  mov ds, bx
  mov es, bx
  mov fs, bx
  mov gs, bx

  popa                     ; Pops edi,esi,ebp...
  add esp, 8     ; Cleans up the pushed error code and pushed ISR number
  sti
  iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP