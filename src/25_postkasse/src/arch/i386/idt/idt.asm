; Makes idt_flush visible to C code
global idt_flush

;idt_flush function
idt_flush:
   mov eax, [esp+4] ; Get the pointer to IDTR struct
   lidt [eax]       ; load the IDTR with the address stores in eax
   ret              ; Return to the c function