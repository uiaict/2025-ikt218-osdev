#pragma once //Gjør at denne filen blir ikke behandlet før programmet compiler for at den skal inkluderes bare en gang.
#include <libc/stdint.h>
#include "../src/arch/i386/print.h"


#define MAX_INTERRUPTS 256 //Antall interrups x86 processoren  vil kunne støtte i IDT-en.
#define MAX_LISTENERS_PER_ISR 4 //begrenser antall funksjoner (listener4s) som kan respondere til et interupt til 4.

//Under er registrert cpuens tilstander ved et interupt:
typedef struct registers {
    uint32_t ds; //Data segment selektor/velger
    uint32_t edi, esi, ebp, esp;  //registre lagret med "pusha"
    uint32_t ebx, edx, ecx, eax; //samme her
    uint32_t int_no/*interrupt-numerering*/, err_code/*feil (kode)*/;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (*interrupt_listener_t)(registers_t* regs);


void isr_init(); // Initialisering av helle ISR-systemet
void subscribe_interrupt(uint8_t interrupt_number, interrupt_listener_t handler); //brukes for å registrere en spesifikk funksjon til å kalles til et interrupt gitt en bestemt tid.
void subscribe_global(interrupt_listener_t handler); // samme men gjelder for ALLE interupts
void isr_dispatch(registers_t* regs); //funskjon som videresender interuptet til alle lytternen.
