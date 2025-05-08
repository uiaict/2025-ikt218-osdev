# IKT218 OS-prosjekt – Avansert operativsystem
 

## Ferdige komponenter

- [x] GDT, IDT og ISR initialisert
- [x] Tastaturinterrupt (IRQ1)
- [x] Shell med flere kommandoer
- [x] Memory manager (`malloc`, `free`)
- [x] Paging aktivert
- [x] PIT driver med `sleep_busy()` og `sleep_interrupt()`
- [x] PC speaker med pianospiller og musikk
- [x] `UiAOS>`-shell med kommandoene:

## Shell-kommandoer

| Kommando     | Funksjon                                        |
|--------------|-------------------------------------------------|
| `help`       | Viser tilgjengelige kommandoer                  |
| `clear`      | Tømmer skjermen                                 |
| `echo [tekst]` | Skriver ut teksten                            |
| `shutdown`   | Slår av (QEMU via port 0x604)                   |
| `play`       | Spiller en forhåndsdefinert melodi              |
| `piano`      | Starter interaktiv pianomodus (trykk ESC for å avslutte) |
| `sleep`      | Kjører `sleep_busy()` og `sleep_interrupt()` for demonstrasjon |

## Demo-funksjoner

Kommandoen `sleep` viser hvordan både busy-wait og interrupt-baserte delays fungerer.  
Tastaturinput og shell fungerer etterpå.

## Teknisk

- Plattform: i386
- Bygges med: CMake, GCC cross compiler
- Bootes i: QEMU


