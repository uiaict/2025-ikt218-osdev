#include "kernel_memory.h"

// En enkel bump-allocator – vi bare flytter en peker fremover
static uint32_t placement_address;

void init_kernel_memory(uint32_t* kernel_end) {
    placement_address = (uint32_t)kernel_end;
}

void* malloc(size_t size) {
    void* addr = (void*)placement_address;
    placement_address += size;
    return addr;
}

void free(void* ptr) {
    // Ikke implementert – ikke nødvendig i early OS
}
#include "printf.h"
void print_hex(uint32_t val) {
    const char* hex = "0123456789ABCDEF";
    printf("0x");
    for (int i = 28; i >= 0; i -= 4) {
        putc(hex[(val >> i) & 0xF]);
    }
}

void print_memory_layout() {
    extern uint32_t end;

    printf("[Memory] Kernel end: ");
    print_hex((uint32_t)&end);
    putc('\n');

    printf("[Memory] Placement address: ");
    print_hex(placement_address);
    putc('\n');
}

