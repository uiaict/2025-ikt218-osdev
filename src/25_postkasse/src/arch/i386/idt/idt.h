#include "libc/stdint.h"

//The structure is 64 bits(8 bytes) long
typedef struct {
	uint16_t    isr_low;                 //Lower 16 bits of "interrupt service routine" address. First part of the address
	uint16_t    kernel_cs;               //Tells the cpu which part of memory to jump to when running the ISR. Usually set to 0x08
	uint8_t     reserved;                //Not used, always set to 0. Reserver for future use
	uint8_t     attributes;              //Setting that tells CPU how to treat the interrupt. Commonly set to 0x8E
	uint16_t    isr_high;                //Upper 16 bits of "interrupt service routine" address. Second part of the address
} __attribute__((packed)) idt_entry_t;   //Packed means no extra space added between fields. "idt_entry_t" is the name of the struct

//IDTR structure
//Holds the address of the IDT and its size
typedef struct {
	uint16_t	limit;                   //Size of the IDT table -1. Tells the CPU how big IDT is
	uint32_t	base;                    //Memory address of the IDT.
} __attribute__((packed)) idtr_t;

static idtr_t idtr;                      //Creates a variable using the IDTR structure from above

void init_idt();						 //Declares the function to initialize the IDT
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags); //Declares the function to set an IDT gate

