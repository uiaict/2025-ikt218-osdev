/* src/kernel.c */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <multiboot2.h>

#include "../terminal.h"
#include "../gdt.h"

#include "kmem.h"
#include "paging.h"
#include "pit.h"
#include "bootinfo.h"
#include "printf.h"

extern void idt_install(void);
extern void pic_remap(int offset1, int offset2);
extern uint32_t end;    // from linker.ld

int main(uint32_t magic, void *mb_addr) {
    gdt_install();
    terminal_initialize();

    pic_remap(0x20, 0x28);
    idt_install();

    init_kernel_memory(&end);
    init_paging();

    /* Multiboot2: first tag sits right after the two 32-bit fields */
    struct multiboot_tag *tag = (void *)((uint8_t *)mb_addr + 8);
    /* find the mmap tag */
    const struct multiboot_tag *raw =
        mb2_find_tag(tag, MULTIBOOT_TAG_TYPE_MMAP);
    const struct multiboot_tag_mmap *mmap_tag =
        (const struct multiboot_tag_mmap *)raw;
    if (mmap_tag) {
        print_memory_layout(mmap_tag, (uint32_t)&end);
    }

    init_pit();
    asm volatile("sti");

    terminal_write("Hello from the IDT-enabled kernel!\n");

    /* quick heap smoke-test */
    void *a = kmalloc(1024, 0);
    void *b = kmalloc(2048, 0);
    kfree(a);
    kfree(b);
    terminal_write("heap test OK\n");

    int counter = 0;
    for (int count = 0; count < 5; count++) {
        printf("[%d]: Sleeping with busy-waiting...\n", counter);
        sleep_busy(1000);
        printf("[%d]: Woke up from busy-waiting\n", counter++);

        printf("[%d]: Sleeping with interrupts...\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Woke up from interrupts\n", counter++);
    }

    printf("done");

    for (;;) {
        asm volatile("hlt");
    }
}
