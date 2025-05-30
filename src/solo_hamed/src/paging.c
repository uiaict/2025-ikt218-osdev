#include "paging.h"
#include "kheap.h"
#include "monitor.h"

// The kernel's page directory
page_directory_t *kernel_directory = 0;

// The current page directory
page_directory_t *current_directory = 0;

// A bitset of frames - used or free
u32int *frames;
u32int nframes;

// Defined in kheap.c
extern u32int placement_address;

// Macros used in the bitset algorithms
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

// Static function to set a bit in the frames bitset
static void set_frame(u32int frame_addr)
{
   u32int frame = frame_addr/0x1000;
   u32int idx = INDEX_FROM_BIT(frame);
   u32int off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(u32int frame_addr)
{
   u32int frame = frame_addr/0x1000;
   u32int idx = INDEX_FROM_BIT(frame);
   u32int off = OFFSET_FROM_BIT(frame);
   frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set
static u32int test_frame(u32int frame_addr)
{
   u32int frame = frame_addr/0x1000;
   u32int idx = INDEX_FROM_BIT(frame);
   u32int off = OFFSET_FROM_BIT(frame);
   return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame
static u32int first_frame()
{
   u32int i, j;
   for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
   {
       if (frames[i] != 0xFFFFFFFF) // nothing free, exit early
       {
           // at least one bit is free here
           for (j = 0; j < 32; j++)
           {
               u32int toTest = 0x1 << j;
               if ( !(frames[i] & toTest) )
               {
                   return i*4*8+j;
               }
           }
       }
   }
   return (u32int)-1; // No free frames
}

// Function to allocate a frame
void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
   if (page->frame != 0)
   {
       return; // Frame was already allocated, return straight away
   }
   else
   {
       u32int idx = first_frame(); // idx is now the index of the first free frame
       if (idx == (u32int)-1)
       {
           // No free frames!
           panic("No free frames available");
       }
       set_frame(idx*0x1000); // this frame is now ours!
       page->present = 1; // Mark it as present
       page->rw = (is_writeable)?1:0; // Should the page be writeable?
       page->user = (is_kernel)?0:1; // Should the page be user-mode?
       page->frame = idx;
   }
}

// Function to deallocate a frame
void free_frame(page_t *page)
{
   u32int frame;
   if (!(frame=page->frame))
   {
       return; // The given page didn't actually have an allocated frame!
   }
   else
   {
       clear_frame(frame); // Frame is now free again
       page->frame = 0x0; // Page now doesn't have a frame
   }
}

// Function to initialize paging
void init_paging()
{
   // The size of physical memory. For the moment we
   // assume it is 16MB big.
   u32int mem_end_page = 0x1000000;

   monitor_write("Setting up frames...\n");
   nframes = mem_end_page / 0x1000;
   frames = (u32int*)kmalloc(INDEX_FROM_BIT(nframes));
   memset((u8int*)frames, 0, INDEX_FROM_BIT(nframes));

   // Let's make a page directory.
   monitor_write("Creating page directory...\n");
   kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
   memset((u8int*)kernel_directory, 0, sizeof(page_directory_t));
   current_directory = kernel_directory;
   
   monitor_write("Mapping kernel heap pages...\n");
   // Map some pages in the kernel heap area.
   // Here we call get_page but not alloc_frame. This causes page_table_t's
   // to be created where necessary. We can't allocate frames yet because they
   // they need to be identity mapped first below, and yet we can't increase
   // placement_address between identity mapping and enabling the heap!
   u32int i = 0;
   for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
       get_page(i, 1, kernel_directory);

   // We need to identity map (phys addr = virt addr) from
   // 0x0 to the end of used memory, so we can access this
   // transparently, as if paging wasn't enabled.
   monitor_write("Identity mapping kernel memory...\n");
   i = 0;
   while (i < placement_address + 0x1000) // Add some extra padding
   {
       // Kernel code is readable but not writeable from userspace.
       alloc_frame(get_page(i, 1, kernel_directory), 1, 0);
       i += 0x1000;
   }
   
   // Now allocate those pages we mapped earlier.
   monitor_write("Allocating kernel heap frames...\n");
   for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
       alloc_frame(get_page(i, 1, kernel_directory), 0, 0);

   // Before we enable paging, we must register our page fault handler.
   monitor_write("Registering page fault handler...\n");
   register_interrupt_handler(14, page_fault);

   // Now, enable paging!
   monitor_write("Enabling paging...\n");
   switch_page_directory(kernel_directory);
   
   // Initialize the kernel heap.
   monitor_write("Creating kernel heap...\n");
   kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 1); // Make it writeable
   
   monitor_write("Paging initialized\n");
}

// Function to switch page directory
void switch_page_directory(page_directory_t *dir)
{
   current_directory = dir;
   asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
   u32int cr0;
   asm volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; // Enable paging!
   asm volatile("mov %0, %%cr0":: "r"(cr0));
}

// Function to get a page
page_t *get_page(u32int address, int make, page_directory_t *dir)
{
   // Turn the address into an index.
   address /= 0x1000;
   // Find the page table containing this address.
   u32int table_idx = address / 1024;
   if (dir->tables[table_idx]) // If this table is already assigned
   {
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else if(make)
   {
       u32int tmp;
       dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
       memset((u8int*)dir->tables[table_idx], 0, 0x1000);
       dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else
   {
       return 0;
   }
}

// Page fault handler
void page_fault(registers_t regs)
{
   // A page fault has occurred.
   // The faulting address is stored in the CR2 register.
   u32int faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   // The error code gives us details of what happened.
   int present   = !(regs.err_code & 0x1); // Page not present
   int rw = regs.err_code & 0x2;           // Write operation?
   int us = regs.err_code & 0x4;           // Processor was in user-mode?
   int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

   // Output an error message.
   monitor_write("PAGE FAULT EXCEPTION ( ");
   if (present) {monitor_write("present ");}
   if (rw) {monitor_write("read-only ");}
   if (us) {monitor_write("user-mode ");}
   if (reserved) {monitor_write("reserved ");}
   monitor_write(") at address 0x");
   monitor_write_hex(faulting_address);
   monitor_write("\n");
   
   // Call the panic function to halt the system
   panic("Page fault occurred");
}

// Function to print memory layout
void print_memory_layout()
{
   monitor_write("Memory Layout:\n");
   monitor_write("  Kernel end address: 0x");
   monitor_write_hex(placement_address);
   monitor_write("\n  Memory size: 16MB\n");
   monitor_write("  Page size: 4KB\n");
   monitor_write("  Total frames: ");
   monitor_write_dec(nframes);
   monitor_write("\n");
}