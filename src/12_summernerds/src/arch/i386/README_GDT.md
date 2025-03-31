

TIL RAPPORT 
(GJØR DETTE FØRST!!)
DERETTER bli ferdig med .c filen 
og begynn på .s, skriv inn GLOBAL 
gdt_flush koden også skal den funke 


For rapporten forklar 0x9A

A GDT is a data structure that is used by intel x86 processors and it generally  defines the characteristics of various memory areas which we would call segments, so we have a big area of memory and we want to divide it up into different pieces and each of those peces has a specified size and access privelege(who can access the memory and what they can do). Thats what segmentation is going to do this. GDT is only specific to x86. we are not unsing the tabell we are just providing it to the processor for it to be able to use for time being and later on what well do we will set up paging similar to a 64 bit processor, which allows more efficient/ (a more grnaular?) manage memory, this is just to get  things initially up, so we have something to use before we have paging in place thats generally what the gdt is going to do for us. A GDT is like an array and inside the array there we have different entries and each entry describes a section of memory. 


GDT brukes til å håndtere minnesegmentering

struct GDTEntry:

- Base: The bases defines the address where the segment begins, and its divided into three parts (low, middle and high) Når vi legger disse tre sammen så for vi tilsammen 32 bit som reprseentere addressen where the segmnet begins.
-Limit: divided int two pieces

base_low, base_middle og base_high
- Disse tre feltene delre opp en 32-minne adresse i mindre biter slik at den passer inni i GDT-strukturen som CPU-en forventer.
 - En GDT kan ikke lagre hele adressen i ett enkelt felt, så vi må dele den opp i tre deler
 - limit_low: er en regel for hvor stort segmentet er, den er 20 bit stor. De siste 4 bitene (0-3) er lagret i granularity. Så tilsammen bestemmer disse hvor langt segmentet strekker seg i minnet.

GDTPointer:
- Holder adressen til GDT og størrelsen på tabellen
- gdt_flush((uint32_t)&gdt_ptr); som ligger i static void init_gdt() i gdt.c. filen, henger sammen med pointeren i gdt.h filen, GDTPointer bruker funksjonen flush() for å laste opp GDT tabellen i CPU-en.

9A = 1001 1010  (i gdt.c filen i void initGDT filen) er access permission

bare ta med gdt filene i assignment 2 


GDT init .c

1 = the top bit is the present bit, iit defines a valid segment, and has to be set to 1. 
0 = DPL er begge nullenen -> og disisse definrerer permission to accessing this particular segment of memory, 0 er den høyste privilegen som er kernelen, så det er kernel code
0 = DPL er begge nullenen
1 = descriptor, if it set to it is either code or data segment. But if clear defines a system segment. den er sett to 1 so its a code or data seg,ent

1 = executable bit. If 0 defines a data segment. If set  to 1 defines a code segment and can be executed. 
0 =  Direction bit, if set to 0 the segment grows. if set to 1, segment grows down. The offset has to be greater than the limit. We want to grow up in memmory, derfot er sen på 0.
1 = Readable bit/ writeable bit
    - For code segment (Readable bit):
        - if set to 1: Read access allowed. Write access is not allowed : 
        - if set to 0: Read access is not allowed.
    
    For data segment (Writeable bit):
        - if set to 0: write access is not allowed
        - if set to 1: write access is allowed. 
        - Read access is always allowed for data segments.
0 = Accessed bit: 

1001  definerer 9


vi må koble IRQene til IDT via assembly stubber

 