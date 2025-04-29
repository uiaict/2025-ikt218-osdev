#define _SIZE_T_DEFINED  // Sett før vi inkluderer andre filer
#include "libc/stdint.h"
#include "memory_manager.h"
#include "display.h"
#include "miscFuncs.h"

// Definer NULL hvis den ikke er definert
#ifndef NULL
#define NULL ((void*)0)
#endif

// Definisjoner for minnehåndtering
#define PAGE_SIZE 4096  // 4KB pages
#define HEAP_START 0x400000  // 4MB - start av heap
#define HEAP_INITIAL_SIZE 0x100000  // 1MB initial heap størrelse

// Memory block struktur for å holde styr på allokerte blokker
typedef struct memory_block {
    long unsigned int size;        // Størrelse på blokken (inkludert header)
    uint8_t is_free;               // 1 hvis blokken er ledig, 0 hvis allokert
    struct memory_block* next;     // Neste blokk i listen
} memory_block_t;

// Globale variabler for minnehåndtering
static memory_block_t* heap_start = NULL;
static uint32_t heap_end = 0;
static uint32_t kernel_end = 0;

// Page directory og tabeller for paging
static uint32_t* page_directory = NULL;
static uint8_t paging_enabled = 0;

/**
 * Initialiserer kernel memory manager
 * 
 * Setter opp kernel heap, starter ved slutten av kjernen
 * Oppretter første ledige minneblokk
 * addr: Peker til slutten av kjernen i minnet (fra linker script)
 */
void init_kernel_memory(uint32_t* addr) {
    kernel_end = (uint32_t)addr;
    
    // Sett opp heap ved HEAP_START
    heap_start = (memory_block_t*)HEAP_START;
    heap_end = HEAP_START + HEAP_INITIAL_SIZE;
    
    // Initialiser første minneblokk
    heap_start->size = HEAP_INITIAL_SIZE - sizeof(memory_block_t);
    heap_start->is_free = 1;
    heap_start->next = NULL;
    
    display_write_color("Memory manager ready\n", COLOR_GREEN);
}

/**
 * Finner en ledig minneblokk av ønsket størrelse
 * 
 * size: Størrelsen på minnet som trengs (i bytes)
 * Returnerer peker til en passende ledig blokk, eller NULL hvis ingen finnes
 */
memory_block_t* find_free_block(long unsigned int size) {
    memory_block_t* current = heap_start;
    
    while (current) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    
    return NULL; // Ingen passende ledig blokk funnet
}

/**
 * Deler en blokk hvis den er mye større enn ønsket størrelse
 * 
 * block: Blokken som skal deles
 * size: Ønsket størrelse
 */
void split_block(memory_block_t* block, long unsigned int size) {
    // Validerer parametere
    if (!block || size == 0 || size > block->size) {
        return;
    }
    
    // Deler bare hvis resten er stor nok for en ny blokk
    // (minst sizeof(memory_block_t) + 32 bytes for data)
    if (block->size >= size + sizeof(memory_block_t) + 32) {
        memory_block_t* new_block = (memory_block_t*)((uint8_t*)block + sizeof(memory_block_t) + size);
        
        // Validerer at ny blokk er innenfor heap grenser
        if ((uint32_t)new_block < heap_end - sizeof(memory_block_t)) {
            // Setter opp den nye blokken
            new_block->size = block->size - size - sizeof(memory_block_t);
            new_block->is_free = 1;
            new_block->next = block->next;
            
            // Oppdaterer nåværende blokk
            block->size = size;
            block->next = new_block;
        }
    }
}

/**
 * Allokerer minne fra kernel heap
 * 
 * size: Størrelse på minne som skal allokeres i bytes
 * Returnerer peker til allokert minne eller NULL hvis allokering feilet
 */
void* malloc(long unsigned int size) {
    if (size == 0) {
        display_write_color("WARNING: Tried to allocate 0 bytes\n", COLOR_YELLOW);
        return NULL;
    }
    if (size > HEAP_INITIAL_SIZE - sizeof(memory_block_t)) {
        display_write_color("ERROR: Requested allocation size too large\n", COLOR_LIGHT_RED);
        return NULL;
    }
    size = (size + 3) & ~3;
    memory_block_t* block = find_free_block(size);
    if (!block) {
        display_write_color("ERROR: No suitable free block found\n", COLOR_LIGHT_RED);
        return NULL;
    }
    if ((uint32_t)block < (uint32_t)heap_start || 
        (uint32_t)block >= heap_end - sizeof(memory_block_t)) {
        display_write_color("ERROR: Invalid block address in malloc\n", COLOR_LIGHT_RED);
        return NULL;
    }
    if (block->size < size || block->size > HEAP_INITIAL_SIZE) {
        display_write_color("ERROR: Invalid block size in malloc\n", COLOR_LIGHT_RED);
        return NULL;
    }
    block->is_free = 0;
    split_block(block, size);
    void* result = (void*)((uint8_t*)block + sizeof(memory_block_t));
    if ((uint32_t)result < (uint32_t)heap_start || 
        (uint32_t)result >= heap_end) {
        display_write_color("ERROR: Invalid pointer generated in malloc\n", COLOR_LIGHT_RED);
        return NULL;
    }
    return result;
}

/**
 * Slår sammen tilstøtende ledige blokker
 * 
 * block: Startblokken å sjekke for sammenføyninger
 */
void merge_free_blocks(memory_block_t* block) {
    while (block && block->next) {
        if (block->is_free && block->next->is_free) {
            // Regner ut total størrelse inkludert header for neste blokk
            block->size += sizeof(memory_block_t) + block->next->size;
            
            // Hopper over neste blokk
            block->next = block->next->next;
        } else {
            // Går til neste blokk
            block = block->next;
        }
    }
}

/**
 * Frigjør tidligere allokert minne
 * 
 * ptr: Peker til minne som tidligere ble allokert med malloc
 */
void free(void* ptr) {
    if (ptr == NULL) {
        display_write_color("WARNING: Tried to free NULL pointer\n", COLOR_YELLOW);
        return;
    }
    memory_block_t* block = (memory_block_t*)((uint8_t*)ptr - sizeof(memory_block_t));
    if ((uint32_t)block < (uint32_t)heap_start || 
        (uint32_t)block >= heap_end - sizeof(memory_block_t)) {
        display_write_color("ERROR: Invalid pointer sent to free()\n", COLOR_LIGHT_RED);
        return;
    }
    if (block->size == 0 || block->size > HEAP_INITIAL_SIZE) {
        display_write_color("ERROR: Corrupted memory block detected in free()\n", COLOR_LIGHT_RED);
        return;
    }
    if (block->is_free) {
        display_write_color("WARNING: Tried to free already freed memory\n", COLOR_YELLOW);
        return;
    }
    block->is_free = 1;
    merge_free_blocks(heap_start);
}

/**
 * Initialiserer paging for kjernen
 * 
 * Setter opp paging med identity mapping for første 8MB av minnet
 */
void init_paging(void) {
    // Paging er allerede satt opp under boot
    paging_enabled = 1;
}

/**
 * Skriver ut memory layout informasjon
 */
void print_memory_layout(void) {
    display_write_color("=== Memory Layout Information ===\n", COLOR_YELLOW);
    display_write_color("Kernel End Address: 0x", COLOR_WHITE);
    display_write_hex(kernel_end);
    display_write("\n");
    display_write_color("Heap Start Address: 0x", COLOR_WHITE);
    display_write_hex((uint32_t)heap_start);
    display_write("\n");
    display_write_color("Heap End Address: 0x", COLOR_WHITE);
    display_write_hex(heap_end);
    display_write("\n");
    display_write_color("Current Heap Size: ", COLOR_WHITE);
    display_write_decimal(heap_end - (uint32_t)heap_start);
    display_write(" bytes\n");
    display_write_color("Paging Status: ", COLOR_WHITE);
    if (paging_enabled) {
        display_write_color("Enabled\n", COLOR_LIGHT_GREEN);
    } else {
        display_write_color("Disabled\n", COLOR_LIGHT_RED);
    }
    display_write_color("Page Directory Address: 0x", COLOR_WHITE);
    display_write_hex((uint32_t)page_directory);
    display_write("\n");
    display_write_color("\n=== Memory Allocation Blocks ===\n", COLOR_YELLOW);
    memory_block_t* current = heap_start;
    int block_count = 0;
    int free_blocks = 0;
    long unsigned int free_memory = 0;
    while (current) {
        block_count++;
        if (current->is_free) {
            free_blocks++;
            free_memory += current->size;
        }
        display_write_color("Block ", COLOR_WHITE);
        display_write_decimal(block_count);
        display_write(": ");
        display_write_color("Address: 0x", COLOR_WHITE);
        display_write_hex((uint32_t)current);
        display_write(", ");
        display_write_color("Size: ", COLOR_WHITE);
        display_write_decimal(current->size);
        display_write(" bytes, ");
        if (current->is_free) {
            display_write_color("Status: Free\n", COLOR_LIGHT_GREEN);
        } else {
            display_write_color("Status: Allocated\n", COLOR_LIGHT_RED);
        }
        current = current->next;
        if (block_count >= 10) {
            display_write_color("... more blocks not shown ...\n", COLOR_GRAY);
            break;
        }
    }
    display_write_color("\nTotal Blocks: ", COLOR_WHITE);
    display_write_decimal(block_count);
    display_write("\n");
    display_write_color("Free Blocks: ", COLOR_WHITE);
    display_write_decimal(free_blocks);
    display_write("\n");
    display_write_color("Free Memory: ", COLOR_WHITE);
    display_write_decimal(free_memory);
    display_write(" bytes\n");
    display_write_color("==============================\n", COLOR_YELLOW);
} 