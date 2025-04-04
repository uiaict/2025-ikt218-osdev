// isr.h
#ifndef ISR_H
#define ISR_H

#include <libc/stdint.h>
#include <libc/stdbool.h>

// Exception and IRQ numbers
#define IDT_EXCEPTION_DE     0    // Divide-by-zero Error
#define IDT_EXCEPTION_DB     1    // Debug
#define IDT_EXCEPTION_NMI    2    // Non-maskable Interrupt
#define IDT_EXCEPTION_BP     3    // Breakpoint
#define IDT_EXCEPTION_OF     4    // Overflow
#define IDT_EXCEPTION_BR     5    // Bound Range Exceeded
#define IDT_EXCEPTION_UD     6    // Invalid Opcode
#define IDT_EXCEPTION_NM     7    // Device Not Available
#define IDT_EXCEPTION_DF     8    // Double Fault
#define IDT_EXCEPTION_TS    10    // Invalid TSS
#define IDT_EXCEPTION_NP    11    // Segment Not Present
#define IDT_EXCEPTION_SS    12    // Stack-Segment Fault
#define IDT_EXCEPTION_GP    13    // General Protection Fault
#define IDT_EXCEPTION_PF    14    // Page Fault
#define IDT_EXCEPTION_MF    16    // x87 Floating-Point Exception
#define IDT_EXCEPTION_AC    17    // Alignment Check
#define IDT_EXCEPTION_MC    18    // Machine Check
#define IDT_EXCEPTION_XF    19    // SIMD Floating-Point Exception

// IRQ numbers
#define IRQ0                32    // Timer
#define IRQ1                33    // Keyboard
#define IRQ2                34    // Cascade for PIC2
#define IRQ3                35    // COM2
#define IRQ4                36    // COM1
#define IRQ5                37    // LPT2
#define IRQ6                38    // Floppy disk
#define IRQ7                39    // LPT1
#define IRQ8                40    // Real-time clock
#define IRQ9                41    // Free for peripherals
#define IRQ10               42    // Free for peripherals
#define IRQ11               43    // Free for peripherals
#define IRQ12               44    // PS/2 mouse
#define IRQ13               45    // FPU
#define IRQ14               46    // Primary ATA hard disk
#define IRQ15               47    // Secondary ATA hard disk

// Define a simple interrupt handler function pointer type
typedef void (*interrupt_handler_t)(uint8_t interrupt_num);

// I/O port functions
void outb(uint16_t port, uint8_t data);
uint8_t inb(uint16_t port);

// Setup and handle ISRs
void isr_init(void);
void register_interrupt_handler(uint8_t interrupt_num, interrupt_handler_t handler);

// External assembly stubs for each interrupt
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void isr32(void);
extern void isr33(void);
extern void isr34(void);
extern void isr35(void);
extern void isr36(void);
extern void isr37(void);
extern void isr38(void);
extern void isr39(void);
extern void isr40(void);
extern void isr41(void);
extern void isr42(void);
extern void isr43(void);
extern void isr44(void);
extern void isr45(void);
extern void isr46(void);
extern void isr47(void);

#endif /* ISR_H */