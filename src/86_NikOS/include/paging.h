#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_TABLE_SIZE 1024
#define PAGE_SIZE 4096

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/string.h"
#include "memory.h"

typedef struct page_entry {
    uint32_t present    : 1;
    uint32_t rw         : 1;
    uint32_t user       : 1;
    uint32_t write_thru : 1;
    uint32_t cache_dis  : 1;
    uint32_t accessed   : 1;
    uint32_t dirty      : 1;
    uint32_t pat        : 1;
    uint32_t global     : 1;
    uint32_t unused     : 3;
    uint32_t addr       : 20;
} page_entry_t;

typedef struct page_table {
    page_entry_t entries[PAGE_TABLE_SIZE];
} page_table_t;

typedef struct page_directory {
    page_table_t* tables[PAGE_DIRECTORY_SIZE];
    uint32_t tables_phys[PAGE_DIRECTORY_SIZE];
    uint32_t physical_addr;
} page_directory_t;