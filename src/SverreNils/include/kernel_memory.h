#pragma once
#include <stdint.h>
#include <stddef.h>

// Initialiserer memory-manageren fra en gitt adresse (fra linker.ld: `&end`)
void init_kernel_memory(uint32_t* kernel_end);

// Allokerer en blokk med minne
void* malloc(size_t size);

// Frigjør minne (valgfritt – kan være tom implementasjon)
void free(void* ptr);
void print_memory_layout();
