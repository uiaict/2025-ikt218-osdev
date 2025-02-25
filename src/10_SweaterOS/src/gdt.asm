section .text

global gdt_flush  ; Gjør funksjonen tilgjengelig for C-koden

gdt_flush:
    ; Laster inn den nye GDT-tabellen.
    ; ESP inneholder returinformasjon fra C-funksjonen som kalte gdt_flush.
    ; Stacken ser slik ut:
    ; [ESP] -> Return address (fra C)
    ; [ESP+4] -> Adresse til gdt_pointer-strukturen
    ; lgdt (Load Global Descriptor Table) tar en peker til en GDT-ptr-struktur.
    lgdt [esp+4]

    ; Setter segmentregisterene til datasegmentet.
    ; Dette er viktig fordi når vi laster en ny GDT, vil de gamle segmentregisterene
    ; fortsatt peke på den gamle GDT-en. Vi må derfor oppdatere dem manuelt.
    mov ax, 0x10  ; 0x10 er offset til datasegmentet i vår GDT (det tredje entryet).
    mov ds, ax    ; Last inn ny data segment descriptor
    mov es, ax    ; Ekstra segment, ofte brukt til strengoperasjoner
    mov fs, ax    ; FS brukes ofte for spesielle datastrukturer i OS
    mov gs, ax    ; GS kan brukes til CPU-spesifikke data (for eksempel TLS i moderne OS)
    mov ss, ax    ; Stack-segmentet må oppdateres slik at CPU vet hvor stacken ligger

    ; Oppdaterer kode-segmentet ved å utføre en "far jump".
    ; Dette er nødvendig fordi CPU-en ikke umiddelbart bruker den nye GDT-en for kodeutførelse.
    ; Ved å hoppe til en ny offset i minnet med en spesifisert segmentselector, tvinger vi CPU-en
    ; til å bruke den nye GDT-en for kode-segmentet.
    ;
    ; 0x08 er offset til kode-segmentet i GDT (andre entryet).
    ; ".flush" er en lokal etikett som vi hopper til, som bare er neste instruksjon.
    jmp 0x08:.flush

.flush:
    ; Returnerer til C-koden etter at CPU-en har begynt å bruke den nye GDT-en
    ret
