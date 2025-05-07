#pragma once
#include <stdint.h>
#include <multiboot2.h>

const struct multiboot_tag* mb2_find_tag(const struct multiboot_tag* first,
                                         uint32_t type);


static inline const struct multiboot_tag_mmap*
mb2_find_mmap(const struct multiboot_tag* first)
{
    return (const struct multiboot_tag_mmap*)
           mb2_find_tag(first, MULTIBOOT_TAG_TYPE_MMAP);
}


void print_memory_layout(const struct multiboot_tag_mmap* mmap_tag,
                         uint32_t kernel_end);
