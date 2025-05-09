#include "idt.h"
#include "terminal.h"
#include "libc/stdint.h"
#include "pit.h"
#include "command.h"
#include "snake_game.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01
#define ICW4_8086    0x01

// Scancode set 1 (standard XT keyboard) to ASCII lookup table
unsigned char scancode_to_ascii[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Define the IDT with 256 entries (maximum possible interrupts)
struct idt_entry idt[256];
struct idt_ptr idtp;

// Global to track if we're in snake game mode
static bool in_snake_game = false;

// Remap the PIC to avoid conflicts with CPU exceptions
void pic_remap() {
    // Send initialization command to both PICs
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // Set vector offsets
    outb(PIC1_DATA, 0x20); // Master PIC: IRQs 0-7 -> 0x20-0x27 (32-39)
    outb(PIC2_DATA, 0x28); // Slave PIC: IRQs 8-15 -> 0x28-0x2F (40-47)

    // Tell Master PIC about the Slave PIC at IRQ2
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // Set PICs to 8086 mode
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // Mask all IRQs (disable them initially)
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

// Function to set an IDT gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = base & 0xFFFF;           // Lower 16 bits of handler address
    idt[num].base_hi = (base >> 16) & 0xFFFF;   // Upper 16 bits of handler address
    idt[num].sel = sel;                         // Kernel segment selector
    idt[num].always0 = 0;                       // Always 0
    idt[num].flags = flags;                     // Flags (e.g., present, privilege)
}

// Function to load the IDT using LIDT instruction
void idt_load() {
    __asm__ __volatile__("lidt (%0)" : : "r" (&idtp));
}

// Function to enable a specific IRQ
void enable_irq(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

// Function to disable a specific IRQ
void disable_irq(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = inb(port) | (1 << irq);
    outb(port, value);
}

// C handler for CPU exceptions (ISRs 0-31)
void isr_handler(struct regs *r) {
    // Handle different CPU exceptions
    switch (r->int_no) {
        case 0:
            writeline("Divide by Zero Exception\n");
            break;
        case 1:
            writeline("Debug Exception\n");
            break;
        case 2:
            writeline("Non Maskable Interrupt Exception\n");
            break;
        case 3:
            writeline("Breakpoint Exception\n");
            break;
        case 4:
            writeline("Into Detected Overflow Exception\n");
            break;
        case 5:
            writeline("Out of Bounds Exception\n");
            break;
        case 6:
            writeline("Invalid Opcode Exception\n");
            break;
        case 7:
            writeline("No Coprocessor Exception\n");
            break;
        case 8:
            writeline("Double Fault Exception\n");
            break;
        case 9:
            writeline("Coprocessor Segment Overrun Exception\n");
            break;
        case 10:
            writeline("Bad TSS Exception\n");
            break;
        case 11:
            writeline("Segment Not Present Exception\n");
            break;
        case 12:
            writeline("Stack Fault Exception\n");
            break;
        case 13:
            writeline("General Protection Fault Exception\n");
            break;
        case 14:
            writeline("Page Fault Exception\n");
            break;
        case 15:
            writeline("Unknown Interrupt Exception\n");
            break;
        case 16:
            writeline("Coprocessor Fault Exception\n");
            break;
        case 17:
            writeline("Alignment Check Exception\n");
            break;
        case 18:
            writeline("Machine Check Exception\n");
            break;
        case 19:
            writeline("Exception 19\n");
            break;
        case 20:
            writeline("Exception 20\n");
            break;
        case 21:
            writeline("Exception 21\n");
            break;
        case 22:
            writeline("Exception 22\n");
            break;
        case 23:
            writeline("Exception 23\n");
            break;
        case 24:
            writeline("Exception 24\n");
            break;
        case 25:
            writeline("Exception 25\n");
            break;
        case 26:
            writeline("Exception 26\n");
            break;
        case 27:
            writeline("Exception 27\n");
            break;
        case 28:
            writeline("Exception 28\n");
            break;
        case 29:
            writeline("Exception 29\n");
            break;
        case 30:
            writeline("Exception 30\n");
            break;
        case 31:
            writeline("Reserved Exception\n");
            break;
        default:
            writeline("Unknown Exception\n");
            break;
    }
}

// C handler for hardware interrupts (IRQs 0-15, mapped to 32-47)
void irq_handler(struct regs *r) {
    // Convert from IDT number to IRQ number
    uint8_t irq = r->int_no - 32;
    
    // Handle different IRQs
    switch (r->int_no) {
        case 32: // IRQ0: Timer
            //writeline("Timer Interrupt called\n");
            pit_tick();
            break;
        case 33: // IRQ1: Keyboard
            // Read scancode from keyboard port
            uint8_t scancode = inb(0x60);
            
            // Check if this is a key press event (not a key release)
            if (!(scancode & 0x80)) {
                // Convert scancode to ASCII character
                char ascii = scancode_to_ascii[scancode];
                
                // If it's a valid character, process it
                if (ascii != 0) {
                    if (in_snake_game) {
                        // Send key to snake game
                        snake_handle_input(ascii);
                    } else {
                        // Process for terminal/command entry
                        if (ascii == '\n' || ascii == '\r') {
                            execute_current_command();
                        } else {
                            terminal_putchar(ascii);
                            append_to_command(ascii);
                        }
                    }
                } else if (scancode == 14) {  // Backspace key
                    if (!in_snake_game) {
                        terminal_backspace();
                        // Also remove character from command buffer if it exists
                        if (command_length > 0) {
                            command_length--;
                            command_buffer[command_length] = 0;
                        }
                    }
                }
            }
            break;
        // Handle other IRQs here...
    }

    // Send End of Interrupt (EOI) signal to the PIC
    if (irq >= 8) {
        outb(PIC2_COMMAND, 0x20); // Send EOI to Slave PIC
    }
    outb(PIC1_COMMAND, 0x20);     // Send EOI to Master PIC
}

// Set the snake game mode flag
void set_snake_game_mode(bool mode) {
    in_snake_game = mode;
}

// Function to install the IDT
void idt_install() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1; // Size of the IDT
    idtp.base = (uint32_t)&idt;                        // Base address of the IDT

    // Clear the IDT by setting all entries to 0
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Set up CPU exception ISRs (0-31)
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    // Remap the PIC
    pic_remap();

    // Set up hardware IRQ handlers (32-47)
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    // Load the IDT
    idt_load();
}