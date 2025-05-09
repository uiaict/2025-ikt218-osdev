#include <libc/stdint.h>

////////////////////////////////////////
// Kernel Entry Wrapper
////////////////////////////////////////

// Forward declaration of the kernel main function
extern int kernel_main_c(uint32_t magic, void* mb_info_addr);

// Entry point called by the bootloader
int main(uint32_t magic, void* mb_info_addr) {
    return kernel_main_c(magic, mb_info_addr);
}
