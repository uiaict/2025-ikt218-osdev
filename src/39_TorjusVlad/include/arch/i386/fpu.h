#pragma once

static inline void init_fpu() {
    asm volatile ("clts");    // Clear Task Switched flag
    asm volatile ("fninit");  // Init x87 FPU
}