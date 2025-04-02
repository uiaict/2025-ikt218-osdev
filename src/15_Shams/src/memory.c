#include <libc/memory.h>
#include <libc/stdint.h>
#include <libc/stdio.h>
#include <libc/terminal.h>

#define KERNEL_HEAP_SIZE 0x100000 // 1 MB

static uint8_t *heap_start;
static uint8_t *heap_end;
static uint8_t *heap_current;

void init_kernel_memory(uint32_t *kernel_end)
{
    heap_start = (uint8_t *)kernel_end;
    heap_end = heap_start + KERNEL_HEAP_SIZE;
    heap_current = heap_start;
}

void *malloc(size_t size)
{
    if (heap_current + size > heap_end)
    {
        return NULL; // Out of memory
    }
    void *allocated = heap_current;
    heap_current += size;
    return allocated;
}

void free(void *ptr)
{
    // Ikke implementert – bump allocator støtter ikke free
    (void)ptr; // Unngå compiler warning
}

static void print_hex(uint32_t value)
{
    char hex_chars[] = "0123456789ABCDEF";
    terminal_write("0x");
    for (int i = 28; i >= 0; i -= 4)
    {
        char c = hex_chars[(value >> i) & 0xF];
        terminal_putc(c);
    }
}


void print_memory_layout()
{
    terminal_write("Heap Start: ");
    print_hex((uint32_t)heap_start);
    terminal_write("\n");

    terminal_write("Heap End: ");
    print_hex((uint32_t)heap_end);
    terminal_write("\n");

    terminal_write("Current: ");
    print_hex((uint32_t)heap_current);
    terminal_write("\n");
}



