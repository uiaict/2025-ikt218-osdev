#include "libc/stddef.h"
#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include "pit.h"
#include "kernel_memory.h"
#include "interrupt.h"

void run_isr_tests(void)
{
    __asm__ volatile("int $0");
    __asm__ volatile("int $1");
    __asm__ volatile("int $2");
}

void run_memory_interrupt_test(void)
{
    static uint32_t counter = 0;

    void *a = malloc(1024);
    void *b = malloc(2048);
    void *c = malloc(4096);
    puts("\nHeap after 3 mallocs:");
    print_heap_blocks();
    free(b);
    puts("\nAfter freeing b:");
    print_heap_blocks();
    void *d = malloc(1024);
    puts("\nAfter reallocating d:");
    print_heap_blocks();
    free(a);
    free(c);
    free(d);

    puts("Triggering ISR tests...\n");
    run_isr_tests();

    printf("[%d]: Busy-wait sleep...\n", counter);
    sleep_busy(1000);
    printf("[%d]: Done.\n", counter++);
    printf("[%d]: Interrupt sleep...\n", counter);
    sleep_interrupt(3000);
    printf("[%d]: Done.\n", counter++);
}
