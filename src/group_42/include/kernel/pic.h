#ifndef PIC_H
#define PIC_H

#include "libc/stdint.h"

/**
 * * @brief Remap the Programmable Interrupt Controller (PIC).
 *
 * The PIC needs to be remapped to avoid conflicts with CPU exceptions.
 * This function changes the interrupt vectors assigned to the PIC's IRQs.
 */
void remap_pic();

/**
 * @brief Send an End-of-Interrupt (EOI) signal to the PIC.
 * @param irq The IRQ number for which to send the EOI.
 */
void send_eoi(uint8_t irq);

#endif