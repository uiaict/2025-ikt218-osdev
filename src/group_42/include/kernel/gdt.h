#ifndef GDT_H
#define GDT_H

/**
 * /**
 * @brief Installs the Global Descriptor Table (GDT).
 *
 * This function sets up the GDT with appropriate segment descriptors and
 * loads it into the processor.
 */
void gdt_install();

#endif