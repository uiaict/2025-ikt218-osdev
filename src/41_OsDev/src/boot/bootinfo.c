// bootinfo.c

#include <driver/include/terminal.h>
#include <libc/stddef.h>
#include <libc/stdint.h>
#include <libc/stdbool.h>
#include <boot/multiboot2.h>
#include <boot/bootinfo.h>

////////////////////////////////////////
// Local Terminal Helpers
////////////////////////////////////////

// Write a single character using terminal_write
static inline void terminal_put(char c) {
    char s[2] = { c, 0 };
    terminal_write(s);
}

// Print a 32-bit unsigned integer in hexadecimal
static void print_hex32(uint32_t v) {
    const char *hex = "0123456789ABCDEF";
    for (int i = 28; i >= 0; i -= 4)
        terminal_put(hex[(v >> i) & 0xF]);
}

// Print a 32-bit unsigned integer in decimal
static void print_dec(uint32_t v) {
    char buf[11];
    int i = 0;
    if (!v) buf[i++] = '0';
    while (v) { buf[i++] = '0' + (v % 10); v /= 10; }
    while (i--) terminal_put(buf[i]);
}

////////////////////////////////////////
// Memory Map Helpers
////////////////////////////////////////

// Translate multiboot memory type to readable string
static const char* type_to_str(uint32_t t) {
    return (t == MULTIBOOT_MEMORY_AVAILABLE)        ? "usable "
         : (t == MULTIBOOT_MEMORY_ACPI_RECLAIMABLE) ? "ACPI  "
         : (t == MULTIBOOT_MEMORY_NVS)              ? "NVS   "
         : (t == MULTIBOOT_MEMORY_BADRAM)           ? "bad   "
         :                                            "resvd ";
}

////////////////////////////////////////
// Multiboot Tag Utility
////////////////////////////////////////

// Find a tag by type in the multiboot2 info structure
const struct multiboot_tag*
mb2_find_tag(const struct multiboot_tag* first, uint32_t type) {
    for (const struct multiboot_tag* tag = first;
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (const void *)((const uint8_t*)tag + ((tag->size + 7) & ~7)))
        if (tag->type == type) return tag;
    return NULL;
}

////////////////////////////////////////
// Memory Layout Display
////////////////////////////////////////

// Pretty-print the physical memory map using multiboot2 info
void print_bootinfo_memory_layout(const struct multiboot_tag_mmap* mmap_tag,
                                  uint32_t kernel_end) {
    if (!mmap_tag) {
        terminal_write("No multiboot memory‑map tag present\n\n");
        return;
    }

    terminal_write("Physical memory map:\n");
    terminal_write(" type      start_addr   end_addr     KiB   MiB\n");
    terminal_write(" ------------------------------------------------\n");

    const struct multiboot_mmap_entry* e =
        (const void*)mmap_tag->entries;

    while ((const uint8_t*)e < (const uint8_t*)mmap_tag + mmap_tag->size) {
        uint32_t start = (uint32_t)e->addr;
        uint32_t end   = (uint32_t)(e->addr + e->len);
        uint32_t kib   = (uint32_t)(e->len >> 10);
        uint32_t mib   = kib >> 10;

        terminal_put(' ');
        terminal_write(type_to_str(e->type));
        terminal_write("  ");
        print_hex32(start); terminal_put(' ');
        print_hex32(end);   terminal_put(' ');

        // Right-align KiB
        for (int pad = 8; pad > 0 && kib < (1u << (4 * pad)); --pad)
            terminal_put(' ');
        print_dec(kib); terminal_put(' ');

        // Right-align MiB
        for (int pad = 5; pad > 0 && mib < (1u << (4 * pad)); --pad)
            terminal_put(' ');
        print_dec(mib); terminal_put('\n');

        e = (const void*)((const uint8_t*)e + mmap_tag->entry_size);
    }

    terminal_write("\nKernel ends at ");
    print_hex32(kernel_end);
    terminal_put('\n'); terminal_put('\n');
}
