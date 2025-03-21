#ifndef IRQ_H
#define IRQ_H

#include <libc/stdint.h>

// Define IRQ numbers
#define IRQ0  0  // Timer
#define IRQ1  1  // Keyboard
#define IRQ2  2  // Cascade for PIC2
#define IRQ3  3  // COM2
#define IRQ4  4  // COM1
#define IRQ5  5  // LPT2
#define IRQ6  6  // Floppy Disk
#define IRQ7  7  // LPT1
#define IRQ8  8  // CMOS Real Time Clock
#define IRQ9  9  // Free / Legacy SCSI / NIC
#define IRQ10 10 // Free / SCSI / NIC
#define IRQ11 11 // Free / SCSI / NIC
#define IRQ12 12 // PS/2 Mouse
#define IRQ13 13 // FPU / Coprocessor / Inter-processor
#define IRQ14 14 // Primary ATA Hard Disk
#define IRQ15 15 // Secondary ATA Hard Disk

// Function to register an IRQ handler
typedef void (*irq_handler_t)(void);
void irq_install_handler(uint8_t irq, irq_handler_t handler);

// Function to unregister an IRQ handler
void irq_uninstall_handler(uint8_t irq);

// IRQ initialization
void irq_init(void);

// C version of interrupt handling functions
void irq_common_handler(uint32_t irq_num);

#endif // IRQ_H