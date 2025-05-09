#ifndef RNG_H
#define RNG_H

void srand(uint32_t seed);
void srand_pit(void);
uint32_t rand(void);
uint32_t rand_range(uint32_t min, uint32_t max);
void rng_seed_xor(uint32_t n);
uint32_t rng_get_seed(void);
uint32_t roll_dice(uint32_t count, uint32_t sides);

#endif