; ISR and IRQ handlers
[GLOBAL isr0]
[GLOBAL isr1]
[GLOBAL isr2]
[GLOBAL isr3]
[GLOBAL isr4]
[GLOBAL isr5]
[GLOBAL isr6]
[GLOBAL isr7]
[GLOBAL isr8]
[GLOBAL isr9]
[GLOBAL isr10]
[GLOBAL isr11]
[GLOBAL isr12]
[GLOBAL isr13]
[GLOBAL isr14]
[GLOBAL isr15]
[GLOBAL isr16]
[GLOBAL isr17]
[GLOBAL isr18]
[GLOBAL isr19]
[GLOBAL isr20]
[GLOBAL isr21]
[GLOBAL isr22]
[GLOBAL isr23]
[GLOBAL isr24]
[GLOBAL isr25]
[GLOBAL isr26]
[GLOBAL isr27]
[GLOBAL isr28]
[GLOBAL isr29]
[GLOBAL isr30]
[GLOBAL isr31]

; IRQs
[GLOBAL irq0]
[GLOBAL irq1]
[GLOBAL irq2]
[GLOBAL irq3]
[GLOBAL irq4]
[GLOBAL irq5]
[GLOBAL irq6]
[GLOBAL irq7]
[GLOBAL irq8]
[GLOBAL irq9]
[GLOBAL irq10]
[GLOBAL irq11]
[GLOBAL irq12]
[GLOBAL irq13]
[GLOBAL irq14]
[GLOBAL irq15]

; Handler functions
[EXTERN isr_handler]
[EXTERN irq_handler]

; Common ISR stub. Saves processor state, sets up for kernel mode
; segments, calls C-level fault handler, and finally restores stack
; frame.
isr_common_stub:
    ; Save registers
    pusha           ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
    
    ; Save data segment
    mov ax, ds
    push eax

    ; Load kernel data segment descriptor
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Call C handler
    call isr_handler

    ; Restore data segment
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Restore registers
    popa
    add esp, 8      ; Cleans up the pushed error code and pushed ISR number
    iret            ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

; Common IRQ stub. Similar to the ISR code
irq_common_stub:
    ; Save registers
    pusha
    
    ; Save data segment
    mov ax, ds
    push eax

    ; Load kernel data segment descriptor
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Call C handler
    call irq_handler

    ; Restore data segment
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Restore registers
    popa
    add esp, 8      ; Cleans up the pushed error code and pushed IRQ number
    iret

; ISR handlers
; We need to create stub handlers for each interrupt
; Some interrupts push an error code onto the stack, but others don't.
; For the ones that don't, we push a dummy error code to make a uniform stack frame.

; ISR 0: Divide By Zero Exception
isr0:
    cli             ; Disable interrupts
    push byte 0     ; Push a dummy error code
    push byte 0     ; Push the interrupt number
    jmp isr_common_stub

; ISR 1: Debug Exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

; ISR 2: Non Maskable Interrupt Exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

; ISR 3: Int 3 (Breakpoint) Exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

; ISR 4: INTO Exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

; ISR 5: Out of Bounds Exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

; ISR 6: Invalid Opcode Exception
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

; ISR 7: Coprocessor Not Available Exception
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

; ISR 8: Double Fault Exception (With Error Code)
isr8:
    cli
    push byte 8     ; Error code already pushed
    jmp isr_common_stub

; ISR 9: Coprocessor Segment Overrun Exception
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

; ISR 10: Bad TSS Exception (With Error Code)
isr10:
    cli
    push byte 10    ; Error code already pushed
    jmp isr_common_stub

; ISR 11: Segment Not Present Exception (With Error Code)
isr11:
    cli
    push byte 11    ; Error code already pushed
    jmp isr_common_stub

; ISR 12: Stack Fault Exception (With Error Code)
isr12:
    cli
    push byte 12    ; Error code already pushed
    jmp isr_common_stub

; ISR 13: General Protection Fault Exception (With Error Code)
isr13:
    cli
    push byte 13    ; Error code already pushed
    jmp isr_common_stub

; ISR 14: Page Fault Exception (With Error Code)
isr14:
    cli
    push byte 14    ; Error code already pushed
    jmp isr_common_stub

; ISR 15: Reserved Exception
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

; ISR 16: Floating Point Exception
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

; ISR 17: Alignment Check Exception
isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub

; ISR 18: Machine Check Exception
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

; ISRs 19-31: Reserved
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub

isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub

isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub

isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub

isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub

isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub

isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub

isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub

isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub

isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub

; IRQ handlers
irq0:
    cli
    push byte 0
    push byte 32
    jmp irq_common_stub

irq1:
    cli
    push byte 0
    push byte 33
    jmp irq_common_stub

irq2:
    cli
    push byte 0
    push byte 34
    jmp irq_common_stub

irq3:
    cli
    push byte 0
    push byte 35
    jmp irq_common_stub

irq4:
    cli
    push byte 0
    push byte 36
    jmp irq_common_stub

irq5:
    cli
    push byte 0
    push byte 37
    jmp irq_common_stub

irq6:
    cli
    push byte 0
    push byte 38
    jmp irq_common_stub

irq7:
    cli
    push byte 0
    push byte 39
    jmp irq_common_stub

irq8:
    cli
    push byte 0
    push byte 40
    jmp irq_common_stub

irq9:
    cli
    push byte 0
    push byte 41
    jmp irq_common_stub

irq10:
    cli
    push byte 0
    push byte 42
    jmp irq_common_stub

irq11:
    cli
    push byte 0
    push byte 43
    jmp irq_common_stub

irq12:
    cli
    push byte 0
    push byte 44
    jmp irq_common_stub

irq13:
    cli
    push byte 0
    push byte 45
    jmp irq_common_stub

irq14:
    cli
    push byte 0
    push byte 46
    jmp irq_common_stub

irq15:
    cli
    push byte 0
    push byte 47
    jmp irq_common_stub