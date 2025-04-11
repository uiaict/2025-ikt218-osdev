 #ifndef MEMORY_H
 #define MEMORY_H
 
 #include "system.h"  // Ensure system-specific types (like uint8_t, uint32_t) are defined
 
 /*==============================================================================
  * Data Structures
  *============================================================================*/
 
 /*
  * Structure representing a memory allocation.
  * - status: 0 indicates free, 1 indicates allocated.
  * - size:   Size of the allocated memory in bytes.
  */
 typedef struct 
 {
     uint8_t status;
     uint32_t size;
 } alloc_t;
 
 /*==============================================================================
  * Kernel Memory Initialization
  *============================================================================*/
 
 /* Initialize kernel memory management.
  * kernel_end: Pointer to the end of the kernel in memory.
  */
 void init_kernel_memory(uint32_t* kernel_end);
 
 /*==============================================================================
  * Paging Operations
  *============================================================================*/
 
 /* Initializes paging. */
 extern void init_paging();
 
 /* Maps a virtual address to a physical address.
  * virt: Virtual address.
  * phys: Physical address.
  */
 extern void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys);
 
 /*==============================================================================
  * Memory Allocation Functions
  *============================================================================*/
 
 /* Allocates memory of given size with page alignment. */
 extern char* pmalloc(size_t size);
 
 /* Allocates memory of given size. */
 extern void* malloc(size_t size);
 
 /* Frees previously allocated memory. */
 extern void free(void *mem);
 
 /*==============================================================================
  * Memory Manipulation Functions
  *============================================================================*/
 
 /* Copies 'num' bytes from source to destination. */
 extern void* memcpy(void* dest, const void* src, size_t num);
 
 /* Sets 'num' bytes starting from ptr to the specified 8-bit value. */
 extern void* memset(void *ptr, int value, size_t num);
 
 /* Sets 'num' 16-bit values starting from ptr to the specified 16-bit value. */
 extern void* memset16(void *ptr, uint16_t value, size_t num);
 
 /*==============================================================================
  * Helper Functions
  *============================================================================*/
 
 /* Prints the current memory layout (for debugging purposes). */
 void print_memory_layout();
 
 #endif // MEMORY_H
 

 /*
 #include "memory.h"

void* memcpy(void* dst, const void* src, uint16_t num)
{
    uint8_t* u8Dst = (uint8_t *)dst;
    const uint8_t* u8Src = (const uint8_t *)src;

    for (uint16_t i = 0; i < num; i++)
        u8Dst[i] = u8Src[i];

    return dst;
}

void * memset(void * ptr, int value, uint16_t num)
{
    uint8_t* u8Ptr = (uint8_t *)ptr;

    for (uint16_t i = 0; i < num; i++)
        u8Ptr[i] = (uint8_t)value;

    return ptr;
}

int memcmp(const void* ptr1, const void* ptr2, uint16_t num)
{
    const uint8_t* u8Ptr1 = (const uint8_t *)ptr1;
    const uint8_t* u8Ptr2 = (const uint8_t *)ptr2;

    for (uint16_t i = 0; i < num; i++)
        if (u8Ptr1[i] != u8Ptr2[i])
            return 1;

    return 0;
}
 */