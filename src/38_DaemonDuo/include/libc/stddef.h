#ifndef _LIBC_STDDEF_H
#define _LIBC_STDDEF_H

// Define standard size type - use the same type as in stdint.h to avoid conflicts
typedef long unsigned int size_t;

// Other standard definitions can go here
#define NULL ((void*)0)

#endif /* _LIBC_STDDEF_H */