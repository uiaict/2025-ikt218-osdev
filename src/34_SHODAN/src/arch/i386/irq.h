#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no;
    uint32_t err_code;
} irq_regs_t;

void irq_remap(void);
void irq_handler(irq_regs_t* regs);

#ifdef __cplusplus
}
#endif
