#include <interrupts.h>
#include <utils.h>
#include <descriptor_table.h>
#include <libc/stdio.h>
#include <libc/stdio.h>

void (*irq_handlers[16])(void) = {0};

void PIC_remap(void)
{
    int offset1 = 32, offset2 = 40;

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
	
	outb(PIC1_DATA, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	// Unmask both PICs.
	outb(PIC1_DATA, 0);
	outb(PIC2_DATA, 0);
}

void init_irq() {
	for (int i = 0; i < IRQ_COUNT; i++) {
		irq_handlers[i] = NULL;
	}
}

void register_irq_handlers(int irq, void (*handler)(void)) {
    if (irq >= 32 && irq < 48) {
        irq_handlers[irq - 32] = handler;
    }
}

void irq_handler(int irq) {
    if (irq_handlers[irq-32] != NULL) {
        irq_handlers[irq-32]();
    }
    // Use correct offset (40) for the second PIC:
    if (irq >= 40) {
        outb(PIC2_COMMAND, PIC_EOI);
    }

    outb(PIC1_COMMAND, PIC_EOI);
}
