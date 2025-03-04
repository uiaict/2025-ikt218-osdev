; Descriptor Tables Assembly Implementation
; Contains functions to load GDT and IDT into the CPU

section .data
debug_pos: dd 480             ; Current position in VGA memory for debug output - line 6 (80*6=480)

section .text
align 4

; Make functions available to C code
global TOSS_GDT
global TOSS_IDT

; Debug function to print a character
print_char:
    ; Save registers
    pusha
    
    ; Set up parameters for writing to VGA memory
    mov edi, 0xB8000        ; VGA memory address
    add edi, [debug_pos]    ; Add current position offset
    
    ; Write character with white on black color
    mov ah, 0x0F            ; White on black
    mov [edi], ax           ; Write character and attribute
    
    ; Update position for next character
    add dword [debug_pos], 2
    
    ; Restore registers
    popa
    ret

; Function to load the Global Descriptor Table
TOSS_GDT:
    ; Debug: print 'G' to indicate GDT is loading
    mov al, 'G'
    call print_char
    
    ; Get the pointer to the GDT, passed as a parameter
    mov eax, [esp+4]
    
    ; Load the GDT
    lgdt [eax]
    
    ; Debug: print 'g' to indicate GDT is loaded
    mov al, 'g'
    call print_char
    
    ; Perform a far jump to update CS
    jmp 0x08:flush_cs
    
flush_cs:
    ; Debug: print 'F' to indicate flush_cs was reached
    mov al, 'F'
    call print_char
    
    ; Update data segment registers
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Debug: print 'f' to indicate flush_cs completed
    mov al, 'f'
    call print_char
    
    ; Return to C code
    ret

; Function to load the Interrupt Descriptor Table
TOSS_IDT:
    ; Debug: print 'I' to indicate IDT is loading
    mov al, 'I'
    call print_char
    
    ; Load IDT - Parameter is a pointer to the IDT descriptor structure
    ; [esp+4] contains the pointer passed from C
    mov eax, [esp+4]
    lidt [eax]
    
    ; Debug: print 'i' to indicate IDT is loaded
    mov al, 'i'
    call print_char
    
    ret
