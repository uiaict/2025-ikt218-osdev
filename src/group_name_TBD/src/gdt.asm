[GLOBAL gdt_flush]  ; Allows the C code to call gdt_flush().

gdt_flush:
   mov eax, [esp+4] ; esp = top of stack, "+4" gets the parameter sent with the function call
   lgdt [eax]       ; Load the new GDT pointer

                    ; Each gdt entry is 8 bytes, kernel code segment is second entry (0x08 offset),
                    ; data segment is third (0x10 offset)
                    
   mov ax, 0x10     ; Reload all data segment selectors                
   mov ds, ax       ; data segment
   mov es, ax       ; extra segment
   mov fs, ax       ; extra segment f
   mov gs, ax       ; extra segment g
   mov ss, ax       ; stack segment
   jmp 0x08:.flush  ; far jump changes CS as well as IP. CS jmp to 0x08, IP jmp to label to continue code execution
.flush:
   ret