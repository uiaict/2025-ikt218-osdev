; ============================================================================
; Interrupt Service Routines (ISRs) for CPU exceptions and hardware interrupts
; ============================================================================
;
; Dette er lavnivå assembly-kode som håndterer CPU exceptions og hardware
; interrupts. Når en interrupt oppstår, hopper CPU-en til den tilsvarende
; rutinen her, som deretter kaller C-funksjoner for videre håndtering.
;
; Filen inneholder:
; 1. Makroer for å definere ISRs og IRQs
; 2. Felles kode for å håndtere alle ISRs og IRQs
; 3. Definisjoner av alle ISRs (0-31) og IRQs (0-15)
; 4. Spesiell håndtering for division by zero

[BITS 32]
section .text

; Eksterne C-funksjoner som håndterer interrupts
extern exception_handler    ; Håndterer CPU exceptions (0-31)
extern irq_handler          ; Håndterer hardware interrupts (32-47)

; ============================================================================
; Makroer for å definere interrupt handlers
; ============================================================================

; Makro for å definere en ISR uten feilkode
; Noen CPU exceptions pusher ikke en feilkode på stacken,
; så vi må pushe en dummy feilkode (0) for å holde stack-layoutet konsistent
%macro ISR_NO_ERROR_CODE 1
global isr%1
isr%1:
    cli                     ; Deaktiver interrupts for å unngå nesting
    push dword 0            ; Push dummy feilkode (0) - sikre at det er en full dword
    push dword %1           ; Push interrupt-nummer - sikre at det er en full dword
    jmp isr_common_stub     ; Hopp til felles handler
%endmacro

; Makro for å definere en ISR med feilkode
; Noen CPU exceptions pusher en feilkode på stacken automatisk,
; så vi trenger ikke å pushe en dummy feilkode
%macro ISR_ERROR_CODE 1
global isr%1
isr%1:
    cli                     ; Deaktiver interrupts for å unngå nesting
    ; CPU pusher feilkode automatisk
    push dword %1           ; Push interrupt-nummer - sikre at det er en full dword
    jmp isr_common_stub     ; Hopp til felles handler
%endmacro

; Makro for å definere en IRQ handler
; IRQs er hardware interrupts som kommer fra PIC
; De er mappet til interrupt 32-47 (IRQ 0-15)
%macro IRQ 2
global irq%1
irq%1:
    cli                     ; Deaktiver interrupts for å unngå nesting
    push dword 0            ; Push dummy feilkode (0) - sikre at det er en full dword
    push dword %2           ; Push interrupt-nummer (32 + IRQ#) - sikre at det er en full dword
    jmp irq_common_stub     ; Hopp til felles handler
%endmacro

; ============================================================================
; Felles kode for alle ISRs (CPU exceptions)
; ============================================================================
;
; Denne koden kjøres for alle CPU exceptions (interrupt 0-31)
; Den:
; 1. Lagrer CPU-tilstanden (registre)
; 2. Setter opp datasegmenter
; 3. Kaller C-funksjonen exception_handler
; 4. Gjenoppretter CPU-tilstanden
; 5. Returnerer fra interruptet
isr_common_stub:
    ; Lagre CPU-tilstand
    pusha                   ; Push alle general-purpose registre (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI)

    ; Lagre datasegment
    mov ax, ds
    push eax

    ; Last inn kernel datasegment
    mov ax, 0x10            ; Kernel datasegment (indeks 2 i GDT, offset 0x10)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Opprett en stack_state struktur direkte på stacken
    ; Vi bruker en fast verdi for CS (0x08) siden det er det vi forventer
    sub esp, 16             ; Alloker plass for stack_state
    mov dword [esp], 0      ; error_code = 0
    mov dword [esp+4], 0    ; eip = 0 (vi trenger ikke den faktiske verdien)
    mov dword [esp+8], 0x08 ; cs = 0x08 (kernel code segment)
    mov dword [esp+12], 0   ; eflags = 0 (vi trenger ikke den faktiske verdien)
    
    ; Push parametre for exception_handler(cpu_state, int_no, stack_state)
    lea eax, [esp]          ; Få peker til vår stack_state
    push eax                ; Push peker til stack_state
    
    mov eax, [esp+44]       ; Få interrupt-nummer fra original lokasjon
    push eax                ; Push int_no
    
    lea eax, [esp+52]       ; Få peker til CPU-tilstand
    push eax                ; Push peker til cpu_state
    
    call exception_handler  ; Kall C-funksjonen
    add esp, 12             ; Rydd opp parametre (3 * 4 bytes)
    
    add esp, 16             ; Fjern vår stack_state struktur
    
    ; Gjenopprett datasegment
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Gjenopprett CPU-tilstand
    popa                    ; Pop alle general-purpose registre
    add esp, 8              ; Fjern error_code og int_no fra stack
    
    ; Spesiell håndtering for division by zero (INT 0)
    ; Vi må hoppe over den feilende instruksjonen for å unngå en uendelig løkke
    cmp dword [esp-8], 0    ; Sammenlign interrupt-nummeret vi nettopp fjernet
    jne .not_div_zero       ; Hvis ikke division by zero, fortsett normalt
    
    ; For division by zero, øk EIP for å hoppe over den feilende instruksjonen
    ; Divisjonsinstruksjoner kan ha variabel lengde (2-6 bytes)
    ; Vi bruker 6 som en sikrere verdi for å sikre at vi hopper over hele instruksjonen
    add dword [esp], 6      ; Legg til 6 til EIP på stacken (hopp over div-instruksjonen)
    
.not_div_zero:
    sti                     ; Aktiver interrupts igjen
    iret                    ; Return from interrupt (popper EIP, CS, EFLAGS)

; ============================================================================
; Felles kode for alle IRQs (hardware interrupts)
; ============================================================================
;
; Denne koden kjøres for alle hardware interrupts (IRQ 0-15, interrupt 32-47)
; Den:
; 1. Lagrer CPU-tilstanden (registre)
; 2. Setter opp datasegmenter
; 3. Kaller C-funksjonen irq_handler
; 4. Gjenoppretter CPU-tilstanden
; 5. Returnerer fra interruptet
irq_common_stub:
    ; Lagre CPU-tilstand
    pusha                   ; Push alle general-purpose registre

    ; Lagre datasegment
    mov ax, ds
    push eax

    ; Last inn kernel datasegment
    mov ax, 0x10            ; Kernel datasegment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Opprett en stack_state struktur direkte på stacken
    ; Vi bruker en fast verdi for CS (0x08) siden det er det vi forventer
    sub esp, 16             ; Alloker plass for stack_state
    mov dword [esp], 0      ; error_code = 0
    mov dword [esp+4], 0    ; eip = 0 (vi trenger ikke den faktiske verdien)
    mov dword [esp+8], 0x08 ; cs = 0x08 (kernel code segment)
    mov dword [esp+12], 0   ; eflags = 0 (vi trenger ikke den faktiske verdien)
    
    ; Push parametre for irq_handler(cpu_state, int_no, stack_state)
    lea eax, [esp]          ; Få peker til vår stack_state
    push eax                ; Push peker til stack_state
    
    mov eax, [esp+44]       ; Få interrupt-nummer fra original lokasjon
    push eax                ; Push int_no
    
    lea eax, [esp+52]       ; Få peker til CPU-tilstand
    push eax                ; Push peker til cpu_state
    
    call irq_handler        ; Kall C-funksjonen
    add esp, 12             ; Rydd opp parametre (3 * 4 bytes)
    
    add esp, 16             ; Fjern vår stack_state struktur
    
    ; Gjenopprett datasegment
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Gjenopprett CPU-tilstand
    popa                    ; Pop alle general-purpose registre
    add esp, 8              ; Fjern error_code og int_no fra stack
    sti                     ; Aktiver interrupts igjen
    iret                    ; Return from interrupt

; ============================================================================
; CPU Exceptions (0-31)
; ============================================================================
;
; Her definerer vi alle CPU exception handlers ved hjelp av makroene over.
; Noen exceptions pusher en feilkode på stacken, andre gjør det ikke.
; Vi bruker riktig makro for hver exception.

; Exceptions uten feilkode
ISR_NO_ERROR_CODE 0    ; Division by Zero - Divisjon med null
ISR_NO_ERROR_CODE 1    ; Debug Exception - Debugging-relatert exception
ISR_NO_ERROR_CODE 2    ; Non-maskable Interrupt - Kan ikke maskeres/ignoreres
ISR_NO_ERROR_CODE 3    ; Breakpoint - Brukt for debugging
ISR_NO_ERROR_CODE 4    ; Overflow - Aritmetisk overflow
ISR_NO_ERROR_CODE 5    ; Bound Range Exceeded - Array-indeks utenfor grenser
ISR_NO_ERROR_CODE 6    ; Invalid Opcode - Ugyldig instruksjon
ISR_NO_ERROR_CODE 7    ; Device Not Available - FPU/MMX/SSE ikke tilgjengelig
ISR_ERROR_CODE    8    ; Double Fault - Feil under håndtering av en annen exception
ISR_NO_ERROR_CODE 9    ; Coprocessor Segment Overrun - Sjelden brukt
ISR_ERROR_CODE    10   ; Invalid TSS - Problem med Task State Segment
ISR_ERROR_CODE    11   ; Segment Not Present - Segment eksisterer ikke
ISR_ERROR_CODE    12   ; Stack-Segment Fault - Problem med stack segment
ISR_ERROR_CODE    13   ; General Protection Fault - Minnebeskyttelsesfeil
ISR_ERROR_CODE    14   ; Page Fault - Feil ved aksess til en side i minnet
ISR_NO_ERROR_CODE 15   ; Reserved - Reservert av Intel
ISR_NO_ERROR_CODE 16   ; x87 Floating-Point Exception - FPU-feil
ISR_ERROR_CODE    17   ; Alignment Check - Minneaksess ikke riktig justert
ISR_NO_ERROR_CODE 18   ; Machine Check - Intern CPU-feil
ISR_NO_ERROR_CODE 19   ; SIMD Floating-Point Exception - SSE/AVX-feil
ISR_NO_ERROR_CODE 20   ; Reserved - Reservert av Intel
ISR_NO_ERROR_CODE 21   ; Reserved - Reservert av Intel
ISR_NO_ERROR_CODE 22   ; Reserved - Reservert av Intel
ISR_NO_ERROR_CODE 23   ; Reserved - Reservert av Intel
ISR_NO_ERROR_CODE 24   ; Reserved - Reservert av Intel
ISR_NO_ERROR_CODE 25   ; Reserved - Reservert av Intel
ISR_NO_ERROR_CODE 26   ; Reserved - Reservert av Intel
ISR_NO_ERROR_CODE 27   ; Reserved - Reservert av Intel
ISR_NO_ERROR_CODE 28   ; Reserved - Reservert av Intel
ISR_NO_ERROR_CODE 29   ; Reserved - Reservert av Intel
ISR_NO_ERROR_CODE 30   ; Reserved - Reservert av Intel
ISR_NO_ERROR_CODE 31   ; Reserved - Reservert av Intel

; ============================================================================
; Spesielle interrupt handlers
; ============================================================================

; Spesiell handler for interrupt 48 (som ser ut til å trigge i systemet ditt)
global isr48
isr48:
    cli
    push dword 0
    push dword 48
    jmp irq_common_stub     ; Rute til IRQ handler i stedet for ISR handler

; Spesiell handler for veldig store interrupt-numre (catch-all)
global isr_unknown
isr_unknown:
    cli
    push dword 0
    push dword 0xFFFFFFFF   ; Bruk en spesiell markør for ukjente interrupts
    jmp irq_common_stub     ; Rute til IRQ handler for å være sikker

; ============================================================================
; Hardware Interrupts (IRQs 0-15, mappet til interrupt 32-47)
; ============================================================================
;
; Her definerer vi alle IRQ handlers ved hjelp av IRQ-makroen.
; Første parameter er IRQ-nummer (0-15).
; Andre parameter er interrupt-nummer (32-47).

IRQ 0, 32    ; Timer (PIT) - Programmable Interval Timer
IRQ 1, 33    ; Keyboard - PS/2 tastatur
IRQ 2, 34    ; Cascade for 8259A Slave controller - Kobling til slave PIC
IRQ 3, 35    ; COM2 - Serieport 2
IRQ 4, 36    ; COM1 - Serieport 1
IRQ 5, 37    ; LPT2 - Parallellport 2
IRQ 6, 38    ; Floppy Disk - Diskettstasjon
IRQ 7, 39    ; LPT1 / Unreliable "spurious" interrupt - Parallellport 1
IRQ 8, 40    ; CMOS Real Time Clock - Sanntidsklokke
IRQ 9, 41    ; Free for peripherals - Ledig for periferienheter
IRQ 10, 42   ; Free for peripherals - Ledig for periferienheter
IRQ 11, 43   ; Free for peripherals - Ledig for periferienheter
IRQ 12, 44   ; PS2 Mouse - PS/2 mus
IRQ 13, 45   ; FPU / Coprocessor / Inter-processor - Flyttallsprosessor
IRQ 14, 46   ; Primary ATA Hard Disk - Primær harddisk
IRQ 15, 47   ; Secondary ATA Hard Disk - Sekundær harddisk 