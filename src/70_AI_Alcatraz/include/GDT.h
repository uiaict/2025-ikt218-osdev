#ifndef GDT_H
#define GDT_H

// GDT Entry structure
struct gdt_entry {
    unsigned short limit_low;   
    unsigned short base_low;    
    unsigned char base_middle;  
    unsigned char access;       
    unsigned char granularity;  
    unsigned char base_high;    
} __attribute__((packed));

// GDT registry structure
struct gdt_ptr {
    unsigned short limit;  
    unsigned int base;     
} __attribute__((packed));

void gdt_init(void); 

#endif 
