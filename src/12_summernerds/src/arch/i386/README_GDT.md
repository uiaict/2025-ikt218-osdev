

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
- gdt_flush((uint32_t)&gdt_ptr); som ligger i static void init_gdt() i gdt.c. filen, henger sammen med pi\onmitrern i gdt.h filen, GDTPointer bruker funksjonen flush() for å laste opp GDT tabellen i CPU-en.

9A = 1001 1010

1 = 
0 =
0 =

1 =
0 =
1 = 
0 =


Bruker IDT for feilhåndteringer
{
    -
}

 