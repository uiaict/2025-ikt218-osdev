#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include "libc/system.h"

#include <multiboot2.h>
#include "libc/gdt_idt_table.h"
#include "libc/gdt.h"
#include "libc/idt.h"
#include "libc/isr.h"
#include "libc/print.h"
#include "pit.h"
#include "interrupts.h"
#include "memory/memory.h"
#include "libc/common.h"
#include "input.h"
#include "Music/song.h"
#include "Music/frequencies.h"
#include "libc/matrix_rain.h"


// Add kernel_main prototype if it exists in another file
int kernel_main();

struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};
extern uint32_t end;

extern volatile uint32_t ticks;

//void test_interrupts() {
  //  uint32_t start = ticks;
   // while (ticks - start < 1000) {
    //    if ((ticks - start) % 100 == 0) {
     //       printf("Tick: %d\n", ticks);
     //   }
   // }
   // printf("Interrupts are working!\n");
//}

int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    print_int(0);  // Pass a dummy value or actual value
    
    init_gdt();
    init_idt();
    init_irq();
    init_interrupts();
    init_kernel_memory(&end);
    init_paging();
    print_memory_layout();
    init_pit();
    asm volatile("int $0x04"); // Trigger a timer interrupt for testing

    printf("Hello %s", "World\n");
    //test_interrupts();
    

    return kernel_main();


}