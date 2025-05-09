global gdt_flush

gdt_flush:
   MOV EAX, [ESP + 4]   ; Get the address of the GDT
   LGDT [EAX]           ; Load the GDT
   
   MOV   EAX, 0x10      ; 0x10 is the offset in the GDT to our data segment
   MOV   DS, AX         ; Load the data segment descriptors
   MOV   ES, AX         
   MOV   FS, AX         
   MOV   GS, AX         
   MOV   SS, AX
   JMP 0x08:.gdt_loaded

.gdt_loaded:
   RET
