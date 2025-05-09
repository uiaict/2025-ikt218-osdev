#include "paging.h"
#include "libc/string.h"
#include "libc/stdint.h"

static page_directory_t boot_directory __attribute__((aligned(4096)));
static page_table_t identity_table __attribute__((aligned(4096)));

page_directory_t* current_directory;

void paging_init() {
    memset(&boot_directory, 0, sizeof(boot_directory));
    memset(&identity_table, 0, sizeof(identity_table));

    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        identity_table.entries[i].present = 1;
        identity_table.entries[i].rw = 1;
        identity_table.entries[i].addr = i;
    }

    boot_directory.tables[0] = &identity_table;
    boot_directory.tables_phys[0] = (uint32_t)&identity_table | 0x3;
    boot_directory.physical_addr = (uint32_t)boot_directory.tables_phys;

    current_directory = &boot_directory;
}

extern void load_page_directory(uint32_t*);
extern void enable_paging();

void enable_virtual_memory() {
    load_page_directory((uint32_t*) current_directory->physical_addr);
    enable_paging();
}
