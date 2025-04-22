// Global operator new
void* operator new(size_t size) 
{
    return malloc(size);
}

void* operator new[](size_t size) 
{
    return malloc(size);
}

// Global operator delete
void operator delete(void* ptr) noexcept 
{
    free(ptr);
}

void operator delete[](void* ptr) noexcept 
{
    free(ptr);
}

// Sized deallocation operators (optional)
void operator delete(void* ptr, size_t size) noexcept 
{
    (void)size; // Unused parameter
    free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept 
{
    (void)size; // Unused parameter
    free(ptr);
}

int kernel_main_c(uint32_t magic, struct multiboot_info* mb_info_addr) {
    // … (initializations of monitor, GDT, IDT, IRQ, etc.)
    
    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();  // Function to display your memory layout

    // Test memory allocation:
    void* some_memory = malloc(12345); 
    void* memory2 = malloc(54321); 
    void* memory3 = malloc(13331);
    char* memory4 = new char[1000]();
    
    // … continue with other initialization (PIT, etc.)
}