#include "memory.h"
#include "terminal.h"
#include "libc/stddef.h"
#include "libc/stdio.h"  // ✅ Needed for printf

#define PAGE_SIZE 4096

// Place paging structures in low memory and align to 4KB
static uint32_t page_directory[1024] __attribute__((aligned(PAGE_SIZE)));
static uint32_t page_table_0[1024] __attribute__((aligned(PAGE_SIZE))); // 0–4MB
static uint32_t page_table_1[1024] __attribute__((aligned(PAGE_SIZE))); // 4–8MB
static uint32_t page_table_2[1024] __attribute__((aligned(PAGE_SIZE))); // 8–12MB

#define HEAP_START 0x00800000 // 8MB
#define HEAP_END   0x00C00000 // 12MB

typedef struct block_meta {
    size_t size;
    struct block_meta* next;
    int free;
} block_meta_t;

static block_meta_t* heap_start = NULL;
static uint32_t* kernel_end_addr = NULL;

// ---------------------- Paging Setup ------------------------

void init_paging(void) {
    for (int i = 0; i < 1024; i++)
        page_directory[i] = 0x00000002;

    for (int i = 0; i < 1024; i++) {
        page_table_0[i] = (i * PAGE_SIZE) | 3;
        page_table_1[i] = ((i + 1024) * PAGE_SIZE) | 3;
        page_table_2[i] = ((i + 2048) * PAGE_SIZE) | 3;
    }

    page_directory[0] = ((uint32_t)page_table_0) | 3;
    page_directory[1] = ((uint32_t)page_table_1) | 3;
    page_directory[2] = ((uint32_t)page_table_2) | 3;

    asm volatile("mov %0, %%cr3" :: "r"(page_directory));

    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

// ---------------------- Heap Setup ------------------------

void init_kernel_memory(uint32_t* kernel_end) {
    kernel_end_addr = kernel_end;
    heap_start = (block_meta_t*)HEAP_START;
    heap_start->size = HEAP_END - HEAP_START - sizeof(block_meta_t);
    heap_start->free = 1;
    heap_start->next = NULL;
}

static block_meta_t* find_free_block(block_meta_t** last, size_t size) {
    block_meta_t* current = heap_start;
    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

void* malloc(size_t size) {
    if (size == 0) return NULL;

    block_meta_t* last = NULL;
    block_meta_t* block = find_free_block(&last, size);
    if (!block) return NULL;

    if (block->size > size + sizeof(block_meta_t)) {
        block_meta_t* new_block = (block_meta_t*)((char*)block + sizeof(block_meta_t) + size);
        new_block->size = block->size - size - sizeof(block_meta_t);
        new_block->free = 1;
        new_block->next = block->next;

        block->size = size;
        block->next = new_block;
    }

    block->free = 0;
    return (void*)(block + 1);
}

void free(void* ptr) {
    if (!ptr) return;

    block_meta_t* block = (block_meta_t*)ptr - 1;
    block->free = 1;

    if (block->next && block->next->free) {
        block->size += sizeof(block_meta_t) + block->next->size;
        block->next = block->next->next;
    }
}

// ---------------------- Memory Layout Output ------------------------

void print_hex32(uint32_t value) {
    char hex[11] = "0x00000000";
    const char* digits = "0123456789ABCDEF";
    for (int i = 9; i >= 2; i--) {
        hex[i] = digits[value & 0xF];
        value >>= 4;
    }
    terminal_write(hex);
    terminal_write("\n");
}

void print_dec(size_t value) {
    char buf[20];
    int i = 0;

    if (value == 0) {
        terminal_write("0\n");
        return;
    }

    while (value && i < 19) {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    for (int j = i - 1; j >= 0; j--) {
        terminal_put_char(buf[j]);
    }
    terminal_write(" bytes\n");
}


void print_memory_layout() {
    uint32_t heap_start_addr = (uint32_t)heap_start;
    uint32_t heap_end_addr = HEAP_END;
    uint32_t total_heap_size = heap_end_addr - heap_start_addr;

    block_meta_t* current = heap_start;
    size_t used = 0;
    size_t free = 0;

    while (current) {
        if (current->free)
            free += current->size;
        else
            used += current->size;

        current = current->next;
    }

    printf("Memory Layout:\n");
    printf("Heap start     : 0x%x\n", heap_start_addr);
    printf("Heap end       : 0x%x\n", heap_end_addr);
    printf("Heap size      : %u bytes\n", total_heap_size);
    printf("Memory used    : %u bytes\n", used);
    printf("Memory free    : %u bytes\n", free);
}


// // Example notes:
// static Note music_1[] = {
//     {440, 500},  // A4 for 0.5 seconds
//     {494, 300},  // B4 for 0.3 seconds
//     {523, 500},  // C5 for 0.5 seconds
//     {0,   200},  // rest for 0.2 seconds
//     {660, 500}   // E5 for 0.5 seconds
// };


// static void play_music_demo() {
//     // Create a Song struct referencing music_1
//     Song mySong;
//     mySong.notes = music_1;
//     mySong.note_count = sizeof(music_1)/sizeof(Note);

//     SongPlayer* player = create_song_player(); // from song.cpp

//     printf("Playing a sample song...\n");
//     player->play_song(&mySong);
//     printf("Finished playing the song.\n");
// }
