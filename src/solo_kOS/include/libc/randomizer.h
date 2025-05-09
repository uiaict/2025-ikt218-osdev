#pragma once
#include "libc/stdint.h"

void  rand_init(void);         // call once at start-up
uint32_t rand_u32(void);       // 0 … 2³¹-1 
uint32_t rand_range(uint32_t max); // 0 … max-1 (simple modulo)
uint32_t rand_range_skip(uint32_t max, const uint32_t *exclude, size_t exclude_cnt); // Like rand_range but skips excluded values