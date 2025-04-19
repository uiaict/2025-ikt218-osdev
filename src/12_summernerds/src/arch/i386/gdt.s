
/*her blir protected mode aktivert, og
setter opp segmentregistrene*/


.global gdt_flush  /*laster in GDT tabellen*/
gdt_flush: 
    lgdt (%eax)        
    mov %cr0, %eax     
    or $1, %eax     /*kan skrive (%la istdet for %eax), her blir protection mode slåt på (går fra 0 bit til 1 bit)   */
    mov %eax, %cr0     

    
    ljmp $0x08, $.protected_mode   /*utføerer et langt hopp, bytter til 32-bit*/
.protected_mode:  /*starter protection mode*/
    
    mov $0x10, %ax    
    mov %ax, %ds  /*data segment*/
    mov %ax, %es  /*extra segment*/
    mov %ax, %fs  /*FS*/
    mov %ax, %gs  /*GS*/
    mov %ax, %ss  /*stack segment*/

    ret   /*retunerer tilbake til C koden*/