#pragma once
#include <stdint.h>
#include <multiboot2.h>

////////////////////////////////////////
// Multiboot2 Tag Utilities
////////////////////////////////////////

// Find the first multiboot tag of a given type
const struct multiboot_tag* mb2_find_tag(const struct multiboot_tag* first,
                                         uint32_t type);

// Convenience: Get memory map tag
static inline const struct multiboot_tag_mmap*
mb2_find_mmap(const struct multiboot_tag* first)
{
    return (const struct multiboot_tag_mmap*)
           mb2_find_tag(first, MULTIBOOT_TAG_TYPE_MMAP);
}

////////////////////////////////////////
// Memory Layout Display
////////////////////////////////////////

// Print memory layout based on multiboot memory map
void print_bootinfor_memory_layout(const struct multiboot_tag_mmap* mmap_tag,
                                   uint32_t kernel_end);
