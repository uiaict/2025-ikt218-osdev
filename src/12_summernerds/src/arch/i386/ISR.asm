; isr.s – Dette er assemblerfilen som definerer interrupt-stubber for IRQ-er
; IRQ0 → 32, IRQ1 (keyboard) → 33, ..., IRQ15 → 47
; Hver "stub" er en funksjon som CPU hopper til når en IRQ skjer

[bits 32]                ; Vi jobber i 32-bit protected mode


;dette er en forenklet versjon, for task 3 og 4



; Forteller linker at denne funksjonen finnes, slik at C-kode kan bruke `extern`
; Her kan du endre navnene hvis du vil, men de må også endres i `idt.c`!
global isr_stub_32
global isr_stub_33
global isr_stub_34
global isr_stub_35
global isr_stub_36
global isr_stub_37
global isr_stub_38
global isr_stub_39
global isr_stub_40
global isr_stub_41
global isr_stub_42
global isr_stub_43
global isr_stub_44
global isr_stub_45
global isr_stub_46
global isr_stub_47

; C-funksjonen som skal håndtere interrupten i C-kode
extern irq_handler

; Én makro som lager en stub – slipper du skrive samme kode 16 ganger
%macro IRQ_STUB 1
isr_stub_%1:
    pusha               ; lagre alle generelle registre
    push dword %1       ; push nummeret til IRQ-en (eks. 33 = keyboard)
    call irq_handler  ; kall C-funksjonen som håndterer IRQ-er
    add esp, 4          ; fjern IRQ-nummeret fra stacken
    popa                ; gjenopprett registre
    iret                ; returner fra interrupt
%endmacro

; Her kaller vi makroen for hver IRQ (fra 32 til 47)
IRQ_STUB 32   ; IRQ0 – system timer
IRQ_STUB 33   ; IRQ1 – tastatur (viktig for at tasturet skal fungere)
IRQ_STUB 34
IRQ_STUB 35
IRQ_STUB 36
IRQ_STUB 37
IRQ_STUB 38
IRQ_STUB 39
IRQ_STUB 40
IRQ_STUB 41
IRQ_STUB 42
IRQ_STUB 43
IRQ_STUB 44
IRQ_STUB 45
IRQ_STUB 46
IRQ_STUB 47
